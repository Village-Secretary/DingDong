#include "TransferData.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::to_string;

inline void isAddZero(std::stringstream& ss_time, uint32_t num)     // 给个位数时间加零
{
	if ((num / 10) < 1)
		ss_time << 0 << num;
	else
		ss_time << num;
}

uint64_t retNowTime(const bool& is_day) // 返回现在的时间
{
	time_t ts = time(NULL);
	tm time = *localtime(&ts);

	std::stringstream ss_time;
	isAddZero(ss_time, time.tm_year + 1900);
	isAddZero(ss_time, time.tm_mon + 1);
	isAddZero(ss_time, time.tm_mday);
	if (!is_day)
	{
		isAddZero(ss_time, time.tm_hour);
		isAddZero(ss_time, time.tm_min);
		isAddZero(ss_time, time.tm_sec);
	}

	return std::stoll(std::string(ss_time.str()));
}

string setAttributes(const string &key, const string & value, const uint32_t& header_max)
{
	string attributes;

	attributes += key + ": " + string(value);
	for (int i = header_max - value.size(); i > 0; --i)
		attributes += "#";
	return attributes;
}
const char * retAttributes(string& value, const uint32_t& header_name, const uint32_t& header_max, const char* buff)
{
	const char * temp = buff;
	for (const char * p = temp + header_name; p != (temp + header_name + header_max); ++p)
	{
		if (*p != '#')
			value.push_back(*p);
	}
	temp += header_name + header_max + 1;

	return temp;
}

string sendData(uint64_t time, const char * message, const MessageData::DATA_TYPE& m_type, const char * id)
{
	string xml;

	xml += "\t<id>" + string(id) + "</id>\n";
	xml += "\t<time>" + to_string(time) + "</time>\n";
	xml += "\t<message type=\"" + MessageData::dataTypeToStr(m_type)+ ">" + string(message) + "</message>\n";

	return xml;
}

const char * parseTypeHeader(uint32_t& type, const char* buff)
{
	const char * temp = buff;

	for (int i = 0; i < HEADER_KEY_TYPE + HEADER_VALUE_TYPE; i++)
		std::cout << temp[i];
	std::cout << std::flush;

	// 读取Type
	string type_str;
	temp = retAttributes(type_str, HEADER_KEY_TYPE, HEADER_VALUE_TYPE, temp);
	type = std::stoll(type_str.c_str());

	return temp;
}

string requestMessage(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, const ID& from, const ID& to, const string& t_data)
{
	string header, data;

	header += setAttributes("Type", "1", HEADER_VALUE_TYPE) + "\n";
	header += setAttributes("From", from.retId(), HEADER_VALUE_ID) + "\n";
	header += setAttributes("To", to.retId(), HEADER_VALUE_ID) + "\n";

	data += "<data type=\"" + transferTypeToStr(type);
	data += "\" status=\"" + transferStatusToStr(status);
	data += "\" from=\"" + transferStatusToStr(status);
	data += "\" group=\"" + transferStatusToStr(status) + "\">\n";
	data += t_data;
	data += "</data>\n";

	header += setAttributes("Length", to_string(data.size()), HEADER_VALUE_LENGTH) + "\n";
	header += "\n";

	return header + data;
}
const char * parseMessageHeader(std::string&from, std::string&to, uint64_t& len, const char* buff)
{
	from = to = "";
	const char * temp = buff;

	temp = retAttributes(from, HEADER_KEY_FROM, HEADER_VALUE_ID, temp);
	temp = retAttributes(to, HEADER_KEY_TO, HEADER_VALUE_ID, temp);

	string len_str;
	temp = retAttributes(len_str, HEADER_KEY_LENGTH, HEADER_VALUE_LENGTH, temp);
	len = std::stoll(len_str.c_str());

	temp += 1;

	return temp;
}

string requestImage(const char*  from, const char*  to, const char*  path)
{
	string header, data;

	// 读取图片
	std::ifstream file_in(path, std::ifstream::binary);
	if (!file_in) return "";
	
	// 循环读取数据
	while (file_in) data.push_back(file_in.get());
	file_in.close();

	header += setAttributes("Type", "2", HEADER_VALUE_TYPE) + "\n";
	header += setAttributes("From", from, HEADER_VALUE_ID) + "\n";
	header += setAttributes("To", to, HEADER_VALUE_ID) + "\n";
	header += setAttributes("Path", path, HEADER_VALUE_PATH) + "\n";
	header += setAttributes("Length", to_string(data.size() - 1), HEADER_VALUE_LENGTH) + "\n";
	header += "\n";

	return header + data;
}
const char * parseImageHeader(std::string& from, std::string& to, std::string& path, uint64_t& len, const char* buff)
{
	from = to = path = "";
	const char * temp = buff;

	temp = retAttributes(from, HEADER_KEY_FROM, HEADER_VALUE_ID, temp);
	temp = retAttributes(to, HEADER_KEY_TO, HEADER_VALUE_ID, temp);
	temp = retAttributes(path, HEADER_KEY_PATH, HEADER_VALUE_PATH, temp);

	string len_str;
	temp = retAttributes(len_str, HEADER_KEY_LENGTH, HEADER_VALUE_LENGTH, temp);
	len = std::stoll(len_str.c_str());

	temp += 1;

	return temp;
}

// 是否足够读取完整请求类型数据
inline bool isEnoughToReadHeaderType(const char * p, const char * end, const uint32_t& temp_len) { return p + HEADER_TYPE_LENGTH - temp_len > end; };
// 是否足够读取完整其他请求属性数据
inline bool isEnoughToReadHeaderAttributes(const uint32_t& type, const char * p, const char * end, const uint32_t& temp_len)
{ 
	if (type == 1) return p + HEADER_MESSAGE_LENGTH - temp_len - HEADER_TYPE_LENGTH > end;
	if (type == 2) return p + HEADER_IMAGE_LENGTH - temp_len - HEADER_TYPE_LENGTH > end;
}
// 将数据暂存到暂存区
void storedInTemporaryArea(const char * p, const char * end, char * temp_buff, uint32_t& temp_len)
{
	for (; p < end; ++p, ++temp_len) 
		temp_buff[temp_len] = *p;
}
// 清零暂存区数据和长度
inline void clearTemporaryArea(char * temp_buff, uint32_t& temp_len, const uint32_t h_length)
{
	temp_len = 0;							// 清零
	memset(temp_buff, '\0', h_length);		// 数组清零
}
// 处理请求文本数据包
const char * processingTextPackets(const char * p, const uint32_t& data_len)
{
	return p + data_len;
}
// 处理请求图片数据包
const char * processingImagePackets(const char * p, const uint32_t& data_len, ofstream& file_out)
{
	for (auto q = p; q < p + data_len; ++q)
		file_out.put(*q);
	
	return p + data_len;
}

#ifdef _WIN32

void recvDDServer(SOCKET fd, char * buff, int len)
{
	uint32_t request_type = 0;												// 请求类型

	static char t_temp_buff[HEADER_TYPE_LENGTH];							// 请求类型暂存区
	uint32_t t_temp_len = 0;										// 请求类型暂存区长度
	static char a_temp_buff[HEADER_IMAGE_LENGTH];							// 请求头属性暂存区
	uint32_t a_temp_len = 0;										// 请求头属性暂存区长度
	clearTemporaryArea(t_temp_buff, t_temp_len, HEADER_TYPE_LENGTH);
	clearTemporaryArea(a_temp_buff, a_temp_len, HEADER_IMAGE_LENGTH);

	string from, to, path;
	uint64_t data_len = 0, data_len_count = 0;
	ofstream file_out;
	uint32_t num = 0;			// 计数器 删除

	//remove("log.txt");
	//ofstream file_log("log.txt", ofstream::binary);

	int recv_len;
	while ((recv_len = recv(fd, buff, len, 0)) >= 0)
	{
		const char * p = buff;							// 迭代指针
		const char * recv_end = buff + recv_len;		// 尾指针

		//file_log << "[" << from << "]" << std::endl;
		//for (int i = 0; i < recv_len; i++)
		//	file_log.put(buff[i]);
		//file_log << "[" << from << "]" << std::endl << std::endl;

		//std::cout << "recv: " << recv_len << " begin: " << static_cast<const void *>(p) << " end: " << static_cast<const void *>(recv_end) << std::endl;

		while (true)
		{
			//std::cout << "is input data?" << std::endl;
			if (!data_len_count)
			{
				//std::cout << "is input attributes?" << std::endl;
				if (!request_type)
				{
					//std::cout << "input type" << std::endl;
					// 判断剩余数据是否足够在读取一个完整 请求类型数据
					if (isEnoughToReadHeaderType(p, recv_end, t_temp_len))
					{
						
						storedInTemporaryArea(p, recv_end, t_temp_buff, t_temp_len);
						if (t_temp_len) 
							std::cout << "type temporary is used" << std::endl;

						break;
					}
					// 请求类型的暂存区不为空时
					if (t_temp_len)
					{
						std::cout << "type temporary is used Now" << std::endl;
						strncat(t_temp_buff, p, HEADER_TYPE_LENGTH - t_temp_len);			// 拷贝数据到暂存区
						parseTypeHeader(request_type, t_temp_buff);							// 读取请求类型数据
						p += (HEADER_TYPE_LENGTH - t_temp_len);								// 调整读取位置p
						clearTemporaryArea(t_temp_buff, t_temp_len, HEADER_TYPE_LENGTH);	// 清零暂存区数据和长度
					}
					else
						p = parseTypeHeader(request_type, p);						// 读取请求类型
				}

				//std::cout << "input attributes" << std::endl;
				// 判断剩余数据是否足够在读取一个完整 请求属性数据
				if (isEnoughToReadHeaderAttributes(request_type, p, recv_end, a_temp_len))
				{
					storedInTemporaryArea(p, recv_end, a_temp_buff, a_temp_len);
					if (a_temp_len) 
						std::cout << "attributes temporary is used" << std::endl;
					break;
				}
				// 请求属性的暂存区不为空时
				if (a_temp_len)
				{
					std::cout << "attributes temporary is used Now" << std::endl;
					// 拷贝buff请求头数据到a_temp_buff中去
					if (request_type == 1)
					{
						strncat(a_temp_buff, buff, HEADER_MESSAGE_LENGTH - a_temp_len);
						parseMessageHeader(from, to, data_len, a_temp_buff);
						p += (HEADER_MESSAGE_LENGTH - a_temp_len);							// 调整读取位置p
					}
					else if (request_type == 2)
					{
						strncat(a_temp_buff, buff, HEADER_IMAGE_LENGTH - a_temp_len);
						parseImageHeader(from, to, path, data_len, a_temp_buff);
						p += (HEADER_IMAGE_LENGTH - a_temp_len);							// 调整读取位置p
					}

					if (request_type == 1) 
						clearTemporaryArea(a_temp_buff, a_temp_len, HEADER_MESSAGE_LENGTH);
					else if (request_type == 2) 
						clearTemporaryArea(a_temp_buff, a_temp_len, HEADER_IMAGE_LENGTH);
				}
				else
				{
					if (request_type == 1) 
						p = parseMessageHeader(from, to, data_len, p);
					if (request_type == 2)
						p = parseImageHeader(from, to, path, data_len, p);
				}
				data_len_count = data_len;			// 记录数据包剩余需要读取的大小


				if (request_type == 2)
				{
					path.replace(46, 6, "server");
					file_out.open(path, std::ofstream::binary);
				}
				if (request_type == 1)
					std::cout << "[" << ++num << "]" << "  From: " << from << "  To: " << to << "  Length: " << data_len << std::endl;
				else if (request_type == 2)
					std::cout << "[" << ++num << "]" << "  From: " << from << "  To: " << to << "  Path: " << path << "  Length: " << data_len << std::endl;
			}
			
			//std::cout << "input data" << std::endl << std::endl;

			// 判断剩余数据是否足够在读取一个完整 请求数据包
			if (p + data_len_count > recv_end)
			{
				// 读取 缓存区剩余数据，然后退出
				data_len_count -= (recv_end - p);	// 记录数据包剩余需要读取的大小
				//std::cout << data_len_count << " data left to read" << std::endl;

				if (request_type == 1) 
					p = processingTextPackets(p, recv_end - p);
				else if (request_type == 2) 
					p = processingImagePackets(p, recv_end - p, file_out);

				break;
			}
			else
			{
				if (request_type == 1) 
					p = processingTextPackets(p, data_len_count);
				else if (request_type == 2)
				{
					p = processingImagePackets(p, data_len_count, file_out);
					file_out.close();
				}

				data_len_count = 0;				// 数据包清零
				request_type = 0;				// 类型清零
			}

		}
	}

	if (recv_len < 0) std::cout << "recv failed! error code: " << WSAGetLastError() << std::endl;
}

#endif