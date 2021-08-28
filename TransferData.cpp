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

DDHeader::DDHeader(const DDHEADER_TYPE& type, const ID& from, const ID& to, const uint64_t& len) : _header_type(type), _header_from(from), _header_to(to), _header_length(len)
{

}

DDHeader::DDHeader(const DDHeader& other) : _header_type(other._header_type), _header_from(other._header_from), _header_to(other._header_from), _header_length(other._header_length)
{

}

DDHeader & DDHeader::operator=(const DDHeader& other)
{
	_header_from = other._header_from;
	_header_to = other._header_to;
	_header_length = other._header_length;

	return *this;
}

const char * DDHeader::parseAttributes(string & value, const uint32_t& header_name, const uint32_t& header_max, const char* buff)
{
	value = "";
	for (const char * p = buff + header_name; p != (buff + header_name + header_max); ++p)
	{
		if (*p != '#') value.push_back(*p);
	}

	return buff + header_name + header_max + 1;
}
string DDHeader::createAttributes(const string &key, const string & value, const uint32_t& header_max)
{
	string attributes;

	attributes += key + ": " + string(value);
	for (int i = header_max - value.size(); i > 0; --i)
		attributes += "#";
	return attributes;
}

string DDHeader::createHeader(const string& t_data)
{
	string header;

	header += createAttributes("Type", std::to_string(static_cast<uint32_t>(_header_type)), HEADER_VALUE_TYPE) + "\n";
	header += createAttributes("From", createIdValueStr(_header_from), HEADER_VALUE_ID) + "\n";
	header += createAttributes("To", createIdValueStr(_header_to), HEADER_VALUE_ID) + "\n";
	header += createAttributes("Length", to_string(t_data.size()), HEADER_VALUE_LENGTH) + "\n";
	header += "\n";

	_header_length = t_data.size();

	return header + t_data;
}
const char * DDHeader::parseTypeHeader(const char* buff)
{
	const char * p = buff;

	// 读取Type
	string type_str;
	p = parseAttributes(type_str, HEADER_KEY_TYPE, HEADER_VALUE_TYPE, p);
	_header_type = static_cast<DDHEADER_TYPE>(std::stoi(type_str.c_str()));

	return p;
}
const char * DDHeader::parseAttributesHeader(const char* buff)
{
	const char * p = buff;
	string temp;

	p = parseAttributes(temp, HEADER_KEY_FROM, HEADER_VALUE_ID, p);
	_header_from.setId(temp.c_str() + 1);
	_header_from.setIdType(parseIdValueStr(temp[0]));

	p = parseAttributes(temp, HEADER_KEY_TO, HEADER_VALUE_ID, p);
	_header_to.setId(temp.c_str() + 1);
	_header_to.setIdType(parseIdValueStr(temp[0]));

	p = parseAttributes(temp, HEADER_KEY_LENGTH, HEADER_VALUE_LENGTH, p);
	_header_length = std::stoll(temp.c_str());

	return p + 1;		// 因为最后有两个换行符
}

string DDHeader::createTextData(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, string xml)
{
	string data;

	data += "<data type=\"" + transferTypeToStr(type) + "\" status=\"" + transferStatusToStr(status) +"\">\n";
	data += xml;
	data += "</data>\n";

	return data;
}
void DDHeader::processingTextData(const std::string & data)
{
	pugi::xml_document messgae_xml;
	pugi::xml_parse_result result_user = messgae_xml.load_string(data.c_str());
	if (!result_user)
	{
		std::cout << "abnormal: xml_document!" << std::endl;
		return;
	}

	pugi::xml_node node_data = messgae_xml.child("data");

	// 查看数据包类型和状态
	TRANSFER_TYPE type = strToTransferType(node_data.attribute("type").value());
	TRANSFER_STATUS status = strToTransferStatus(node_data.attribute("status").value());

	// 根据数据包类型和状态做出相应处理
	switch (type)
	{
	case _login:
		break;
	case _synchr:
		break;
	case _send:
		break;
	case _add:
		break;
	default:
		break;
	}

}

string DDHeader::createImageData(const char * path, const char * name, const char * suffix, const uint8_t& suffix_len)
{
	ifstream file_in(string(path) + name + "." + suffix, ifstream::binary);
	if (!file_in)
	{
		std::cout << "abnormal: ifstream!" << std::endl;
		return "";
	}

	string data;
	// 循环读取数据
	while (file_in) data.push_back(file_in.get());
	file_in.close();

	return to_string(suffix_len) + suffix + data;
}
void DDHeader::processingImageData(const std::string & data)
{
	if (data.size() == 0) return;

	// 知道图片数据要存储在哪里
	string path, name, suffix;
	
	// 选择图片存储路径[私聊消息/群消息/用户头像/群头像]
	// 写一个函数
	path = "C:\\Users\\ZombieProcess\\Desktop\\DingDong\\image\\server\\";

	name = giveAName();			// 取名字

	// 获取图片后缀
	uint8_t suffix_len = data[0] - 48;
	for (uint8_t i = 1; i < suffix_len + 1; i++) suffix.push_back(data[i]);

	// 创建存储改图片
	ofstream image_file(path + name + "." + suffix, ofstream::binary);
	if (!image_file)
	{
		std::cout << "abnormal: ofstream!" << std::endl;
		return;
	}
	
	// 写入数据
	for (auto p = data.begin() + suffix_len + 1; p != data.end(); ++p)
		image_file.put(*p);
	image_file.close();

	// 如果是服务器：转发此图片数据

}

string DDHeader::giveAName(void)
{
	static uint64_t old_time = 0, increment = 0;

	uint64_t time;
	if (time = retNowTime() == old_time)
		increment += 1;
	else
	{
		old_time = time;
		increment = 0;
	}

	return to_string(time) + "_" + to_string(increment);
}

// 选择路径
string choosePath(void);

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

#ifdef _WIN32

void recvDDServer(SOCKET fd, char * buff, int len)
{
	static char t_temp_buff[HEADER_TYPE_LENGTH];					// 请求类型暂存区
	uint32_t t_temp_len = 0;										// 请求类型暂存区长度
	static char a_temp_buff[HEADER_ATTRIBUTES_LENGTH];				// 请求头属性暂存区
	uint32_t a_temp_len = 0;										// 请求头属性暂存区长度
	clearTemporaryArea(t_temp_buff, t_temp_len, HEADER_TYPE_LENGTH);
	clearTemporaryArea(a_temp_buff, a_temp_len, HEADER_ATTRIBUTES_LENGTH);

	uint64_t data_len_count = 0, num = 0;
	DDHeader dd_header;
	string data_buff;

	int recv_len;
	while ((recv_len = recv(fd, buff, len, 0)) >= 0)
	{
		const char * p = buff;							// 迭代指针
		const char * recv_end = buff + recv_len;		// 尾指针

		while (true)
		{
			if (!data_len_count)
			{
				if (!a_temp_len)
				{
					// 判断剩余数据是否足够在读取一个完整 请求类型数据
					if (p + HEADER_TYPE_LENGTH - t_temp_len > recv_end)
					{
						storedInTemporaryArea(p, recv_end, t_temp_buff, t_temp_len);
						break;
					}
					// 请求类型的暂存区不为空时
					if (t_temp_len)
					{
						strncat(t_temp_buff, p, HEADER_TYPE_LENGTH - t_temp_len);			// 拷贝数据到暂存区
						dd_header.parseTypeHeader(t_temp_buff);								// 读取请求类型数据
						p += (HEADER_TYPE_LENGTH - t_temp_len);								// 调整读取位置p
						clearTemporaryArea(t_temp_buff, t_temp_len, HEADER_TYPE_LENGTH);	// 清零暂存区数据和长度
					}
					else
						p = dd_header.parseTypeHeader(p);						// 读取请求类型数据
				}

				// 判断剩余数据是否足够在读取一个完整 请求属性数据
				if (p + HEADER_ATTRIBUTES_LENGTH - a_temp_len > recv_end)
				{
					storedInTemporaryArea(p, recv_end, a_temp_buff, a_temp_len);
					break;
				}
				// 请求属性的暂存区不为空时
				if (a_temp_len)
				{
					strncat(a_temp_buff, buff, HEADER_ATTRIBUTES_LENGTH - a_temp_len);		// 拷贝数据到暂存区
					dd_header.parseAttributesHeader(a_temp_buff);							// 读取请求属性数据
					p += HEADER_ATTRIBUTES_LENGTH - a_temp_len;								// 调整读取位置p
					clearTemporaryArea(a_temp_buff, a_temp_len, HEADER_ATTRIBUTES_LENGTH);	// 清零暂存区数据和长度
				}
				else
					p = dd_header.parseAttributesHeader(p);				// 读取请求属性数据
				data_len_count = dd_header.retLength();					// 记录数据包剩余需要读取的大小
				
				std::cout << "[" << num << "]"
					<< " Type: " << dd_header.retType()
					<< " From: " << dd_header.retFromId().retId() 
					<< " To: " << dd_header.retToId().retId() 
					<< " Length:" << dd_header.retLength() << std::endl;
				num += 1;
			}
			

			// 判断剩余数据是否足够在读取一个完整 请求数据包
			if (p + data_len_count > recv_end)
			{
				data_buff += string(p, recv_end - p);		// 暂存数据
				data_len_count -= (recv_end - p);			// 记录还需读取的数据大小

				break;
			}
			else
			{
				data_buff += string(p, data_len_count);

				if (dd_header.retType() == DDHeader::message) dd_header.processingTextData(data_buff);
				if (dd_header.retType() == DDHeader::image) dd_header.processingImageData(data_buff);

				p += data_len_count;			// 调整读取位置p
				data_buff.clear();				// 清理数据缓存
				data_len_count = 0;				// 数据包计数器清零
			}

		}
	}

	if (recv_len < 0) std::cout << "recv failed! error code: " << WSAGetLastError() << std::endl;
}


#endif