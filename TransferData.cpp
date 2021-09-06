//#define __DD_SERVER
#define __DD_CLIENT

#include "TransferData.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <initializer_list>

using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::to_string;

inline void isAddZero(stringstream& ss_time, uint32_t num)     // 给个位数时间加零
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

string DDHeader::createHeader(const string& t_data)
{
	string header, attributes;

	attributes = std::to_string(static_cast<uint32_t>(_header_type));
	fillPoundKey(attributes, HEADER_VALUE_TYPE - attributes.size());
	header += "TYPE: " + attributes;
	header += "\n";

	attributes = createIdValueStr(_header_from);
	fillPoundKey(attributes, HEADER_VALUE_ID - attributes.size());
	header += "FROM: " + attributes;
	header += "\n";

	attributes = createIdValueStr(_header_to);
	fillPoundKey(attributes, HEADER_VALUE_ID - attributes.size());
	header += "TO: " + attributes;
	header += "\n";

	attributes = to_string(t_data.size());
	fillPoundKey(attributes, HEADER_VALUE_LENGTH - attributes.size());
	header += "LENGTH: " + attributes;
	header += "\n\n";

	_header_length = t_data.size();

	return header + t_data;
}
const char * DDHeader::parseTypeHeader(const char* buff)
{
	const char * p = buff;
	string value;

	p = findDataInPoundKey(value, HEADER_KEY_TYPE, HEADER_VALUE_TYPE, p) + 1;
	_header_type = static_cast<DDHEADER_TYPE>(std::stoi(value.c_str()));

	return p;
}
const char * DDHeader::parseAttributesHeader(const char* buff)
{
	const char * p = buff;
	string value;

	// 因为每行属性后面都会跟一个换行符，所以需要将findDataInPoundKey返回结果加1
	p = findDataInPoundKey(value, HEADER_KEY_FROM, HEADER_VALUE_ID, p) + 1;
	_header_from.setIdType(parseIdValueStr(value[0]));
	_header_from.setId(value.c_str() + 1);

	p = findDataInPoundKey(value, HEADER_KEY_TO, HEADER_VALUE_ID, p) + 1;
	_header_to.setIdType(parseIdValueStr(value[0]));
	_header_to.setId(value.c_str() + 1);

	p = findDataInPoundKey(value, HEADER_KEY_LENGTH, HEADER_VALUE_LENGTH, p) + 1;
	_header_length = std::stoll(value.c_str());

	return p + 1;		// 因为最后有两个换行符，所以这里也需要加1
}

void DDHeader::textDataHandler(const std::string & data, TRANSFER_TYPE& type, TRANSFER_STATUS& status)
{
	pugi::xml_document messgae_xml;
	pugi::xml_parse_result result_user = messgae_xml.load_string(data.c_str());
	if (!result_user)
	{
		std::cout << "abnormal: xml_document![textDataHandler]" << std::endl;
		return;
	}

	pugi::xml_node node_data = messgae_xml.child("data");

	// 查看数据包类型和状态
	type = strToTransferType(node_data.attribute("type").value());
	status = strToTransferStatus(node_data.attribute("status").value());
}

string DDHeader::createImageData(const char * path, const char * imagefile, const TRANSFER_TYPE& operate)
{

	ifstream file_in(string(path) + imagefile, ifstream::binary);
	if (!file_in)
	{
		std::cout << "abnormal: ifstream![createImageData]" << std::endl;
		return "";
	}

	string data;
	// 循环读取数据
	while (file_in) data.push_back(file_in.get());
	file_in.close();

	string s_imagefile(imagefile), s_type(transferTypeToStr(operate));
	fillPoundKey(s_imagefile, IMAGE_FILE_LENGTH - s_imagefile.size());
	fillPoundKey(s_type, TRANSFER_TYPE_STRING_LENGTH - s_type.size());


	// 如果文件名和后缀大于IMAGE_FILE_LENGTH，则创建失败
	if (s_imagefile.size() > IMAGE_FILE_LENGTH)
	{
		std::cout << "abnormal: IMAGE_FILE_LENGTH![createImageData]" << std::endl;
		return "";
	}

	return s_imagefile + s_type + data;
}
void DDHeader::imageDataHandler(const string & data, string& path, string& imagefile, TRANSFER_TYPE& operate)
{
	if (data.size() == 0) return;

	string s_type;

	// 获取图片后缀和图片操作
	findDataInPoundKey(s_type, 0, TRANSFER_TYPE_STRING_LENGTH, findDataInPoundKey(imagefile, 0, IMAGE_FILE_LENGTH, data.data()));

	operate = strToTransferType(s_type);
	path = choosePath(*this, operate);			// 根据图片操作选择路径

	// 创建存储改图片
	ofstream file(path + imagefile, ofstream::binary);
	if (!file)
	{
		std::cout << "abnormal: ofstream![imageDataHandler]" << std::endl;
		return;
	}

	// 写入数据 （注：减1是因为存储的的时候，接受时string类型自动加的那个空字符）
	for (auto p = data.begin() + IMAGE_FILE_LENGTH + TRANSFER_TYPE_STRING_LENGTH; p != data.end() - 1; ++p)
		file.put(*p);
	file.close();

}

void DDHeader::fillPoundKey(string &data, const uint32_t& max)
{
	for (int i = max; i > 0; --i)
		data += "#";
}
const char * DDHeader::findDataInPoundKey(string & data, const uint32_t& bg, const uint32_t& ed, const char* buff)
{
	data = "";
	for (const char * p = buff + bg; p != (buff + bg + ed); ++p)
	{
		if (*p != '#') data.push_back(*p);
	}

	return buff + bg + ed;
}

string giveAName(void)
{
	static uint64_t old_time = 0, increment = 0;

	uint64_t time;
	if ((time = retNowTime()) == old_time)
		increment += 1;
	else
	{
		old_time = time;
		increment = 0;
	}

	return to_string(time) + "_" + to_string(increment);
}

string choosePath(const DDHeader& header, const TRANSFER_TYPE& operate)
{
	switch (operate)
	{
	case TRANSFER_TYPE::_send:
		if (header.retToId().retIdType() == ID::user) return IMAGE_USER_MESSAGE;
		if (header.retToId().retIdType() == ID::group) return IMAGE_GROUP_MESSAGE;
		break;
	case TRANSFER_TYPE::_setting:
		if (header.retToId().retIdType() == ID::user) return IMAGE_USER_AVATAR;
		if (header.retToId().retIdType() == ID::group) return IMAGE_GROUP_AVATAR;
		break;
	case TRANSFER_TYPE::_add:
		if (header.retToId().retIdType() == ID::user) return IMAGE_USER_AVATAR;
		if (header.retToId().retIdType() == ID::group) return IMAGE_GROUP_AVATAR;
		break;
	}
}

string getSuffix(const string& file) { return file.substr(file.find_last_of('.') + 1); };

bool toOtherPath(const char * old_path, const char * new_path)
{
	string image_data;
	ifstream file_in(old_path, ofstream::binary);
	if (!file_in)
	{
		std::cout << "abnormal: ifstream![toOtherPath]" << std::endl;
		return false;
	}

	// 读取数据
	while (file_in) { image_data.push_back(file_in.get()); };
	file_in.close();

	ofstream file_out(new_path, ofstream::binary);
	if (!file_out)
	{
		std::cout << "abnormal: ofstream![toOtherPath]" << std::endl;
		return false;
	}

	// 写入数据
	for (auto p = image_data.begin(); p != image_data.end() - 1; ++p)
		file_out.put(*p);
	file_out.close();

	return true;
}

bool setImageToRightPath(const DDHeader& header, const char * oldpath, const TRANSFER_TYPE& operate, string& newpath, string& imagefile)
{
	newpath = choosePath(header, operate);		// 获取路径
	imagefile = giveAName();					// 获取名字
	imagefile += "." + getSuffix(oldpath);		// 获取后缀

	if (!toOtherPath(oldpath, (newpath + imagefile).c_str()))	// 移动文件
	{
		std::cout << "abnormal: toOtherPath![setImageToRightPath]" << std::endl;
		return false;
	}

	return true;
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


#include <cstdio>

// 接受数据
void ddRecv(dd_socket fd, char * buff, int len)
{
	static char t_temp_buff[HEADER_TYPE_LENGTH];					// 请求类型暂存区
	uint32_t t_temp_len = 0;										// 请求类型暂存区长度
	static char a_temp_buff[HEADER_ATTRIBUTES_LENGTH];				// 报文头属性暂存区
	uint32_t a_temp_len = 0;										// 报文头属性暂存区长度
	clearTemporaryArea(t_temp_buff, t_temp_len, HEADER_TYPE_LENGTH);
	clearTemporaryArea(a_temp_buff, a_temp_len, HEADER_ATTRIBUTES_LENGTH);

	uint64_t data_len_count = 0, num = 0;
	DDHeader dd_header;
	string data_buff;
	bool isType = true;

	int recv_len;
	while ((recv_len = recv(fd, buff, len, 0)) >= 0)
	{
		const char * p = buff;							// 迭代指针
		const char * recv_end = buff + recv_len;		// 尾指针

		while (true)
		{
			if (!data_len_count)
			{
				if (isType)
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
					isType = false;
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
				data_buff.insert(data_buff.end(), p, recv_end);	// 暂存数据
				data_len_count -= (recv_end - p);	// 记录还需读取的数据大小

				break;
			}
			else
			{
				data_buff.insert(data_buff.end(), p, p + data_len_count);

				if (dd_header.retType() == DDHeader::message)
				{
					TRANSFER_TYPE type;
					TRANSFER_STATUS status;
					dd_header.textDataHandler(data_buff, type, status);
					std::cout << "xml: "
						<< " [type]" << transferTypeToStr(type)
						<< " [status]" << transferStatusToStr(status) << std::endl
						<< data_buff << std::endl << std::endl;
				}
				if (dd_header.retType() == DDHeader::image)
				{
					string path, imagefile;
					TRANSFER_TYPE operate;
					dd_header.imageDataHandler(data_buff, path, imagefile, operate);
					std::cout << "image: "
						<< " [path]" << path
						<< " [iamgefile]" << imagefile
						<< " [operate]" << transferTypeToStr(operate) << std::endl << std::endl;
				}

				p += data_len_count;			// 调整读取位置p
				data_buff.clear();				// 清理数据缓存
				data_len_count = 0;				// 数据包计数器清零
				isType = true;
			}

		}
	}

	if (recv_len < 0) std::cout << "recv failed! error code: " << WSAGetLastError() << std::endl;
}

void ddSend(dd_socket fd)
{
	ID myself("2248585019", ID::user);
	ID to_user("963125018", ID::group);
	std::string temp_path = "C:\\Users\\ZombieProcess\\Desktop\\DingDong\\image\\client\\";
	int count = 0;
	while (true)
	{
		uint32_t num = 0, header_type = 0, trans_type;
		
		do {
			printf("enter a number to header type[message(0) image(1)]: ");
			scanf(" %d", &header_type);
		} while (header_type < 0 || header_type > 1);

		if (header_type == 0)
		{
			do {
				printf("enter a number to header type[register = 0, heart = 2, login = 4, setting = 6, send = 8, add = 10, delete = 12, request = 14]: ");
				scanf(" %d", &trans_type);
			} while ((trans_type < 0 || trans_type > 12) && (trans_type % 2 == 1));
		}
		else if (header_type == 1)
		{
			do {
				printf("enter a number to header type[setting = 6, send = 8, add = 10]: ");
				scanf(" %d", &trans_type);
			} while ((trans_type < 6 || trans_type > 10) && (trans_type % 2 == 1));
		}

		printf("enter a number to send number: ");
		scanf(" %d", &num);
		if (header_type == 1 && num > 15) num = 45;		// 限制图片发送【文件夹只有45张图片】

		for (int i = 0; i < num; ++i)
		{
			int send_len = 0;
			DDHeader m_header(DDHeader::DDHEADER_TYPE::message, myself, to_user);
			DDHeader i_header(DDHeader::DDHEADER_TYPE::image, myself, to_user);

			if (header_type == 0)
			{
				string data;
				switch (trans_type)
				{
				case TRANSFER_TYPE::_register:break;
				case TRANSFER_TYPE::_login:break;
				case TRANSFER_TYPE::_heart:break;
				case TRANSFER_TYPE::_setting:
					GroupData group_1(to_user.retId(), "HappyFamily", "./image/group_avatar/32.jpg", 20210812, "XXXXXXXXXX", "2248585019", 3);
					data = m_header.createTextData(TRANSFER_TYPE::_setting, TRANSFER_STATUS::_request, group_1);
					break;
				case TRANSFER_TYPE::_send:
					MessageData message(42122, MessageData::DATA_TYPE::text, myself.retId(), retNowTime(), "Hello");
					data = m_header.createTextData(TRANSFER_TYPE::_send, TRANSFER_STATUS::_request, message);
					break;
				case TRANSFER_TYPE::_add:
					data = m_header.createTextData(TRANSFER_TYPE::_add, TRANSFER_STATUS::_request, group_1);
					break;
				case TRANSFER_TYPE::_delete:break;
				case TRANSFER_TYPE::t_request:break;
				}

				std::string send_data = m_header.createHeader(data);

				send_len = send(fd, send_data.c_str(), send_data.size(), 0);
				if (send_len != send_data.size())
					printf("error: inconsistent! send length: %d,  data length: %d\n", send_len, send_data.size());
			}
			else if (header_type == 1)
			{
				std::string path, imagefile;

				string data_text, data_image;
				switch (trans_type)
				{
				case TRANSFER_TYPE::_register:break;
				case TRANSFER_TYPE::_login:break;
				case TRANSFER_TYPE::_heart:break;
				case TRANSFER_TYPE::_setting:
					// 复制图片到正确路径
					setImageToRightPath(m_header, (temp_path + std::to_string(i) + ".jpg").c_str(), TRANSFER_TYPE::_setting, path, imagefile);
					GroupData group_1(to_user.retId(), "HappyFamily", (path + imagefile).c_str(), 20210812, "XXXXXXXXXX", "2248585019", 3);
					data_text = m_header.createTextData(TRANSFER_TYPE::_setting, TRANSFER_STATUS::_request, group_1);
					data_image = i_header.createImageData(path.c_str(), imagefile.c_str(), TRANSFER_TYPE::_setting);
					break;
				case TRANSFER_TYPE::_send:
					// 复制图片到正确路径
					setImageToRightPath(m_header, (temp_path + std::to_string(i) + ".jpg").c_str(), TRANSFER_TYPE::_send, path, imagefile);
					MessageData message(42122, MessageData::DATA_TYPE::text, myself.retId(), retNowTime(), (path + imagefile).c_str());
					data_text = m_header.createTextData(TRANSFER_TYPE::_send, TRANSFER_STATUS::_request, message);
					data_image = i_header.createImageData(path.c_str(), imagefile.c_str(), TRANSFER_TYPE::_send);
					break;
				case TRANSFER_TYPE::_add:
					path = IMAGE_GROUP_AVATAR;
					imagefile = "0.jpg";
					group_1 = GroupData(to_user.retId(), "HappyFamily", (path + imagefile).c_str(), 20210812, "XXXXXXXXXX", "2248585019", 3);
					data_text = m_header.createTextData(TRANSFER_TYPE::_add, TRANSFER_STATUS::_request, group_1);
					data_image = i_header.createImageData(path.c_str(), imagefile.c_str(), TRANSFER_TYPE::_add);
					break;
				case TRANSFER_TYPE::_delete:break;
				case TRANSFER_TYPE::t_request:break;
				}

				// 添加报文头内容
				std::string send_data = m_header.createHeader(data_text);

				send_len = send(fd, send_data.c_str(), send_data.size(), 0);
				if (send_len != send_data.size())
					printf("error: inconsistent! send length: %d,  data length: %d\n", send_len, send_data.size());

				// 添加报文头内容
				send_data = i_header.createHeader(data_image);
				send_len = send(fd, send_data.c_str(), send_data.size(), 0);
				if (send_len != send_data.size())
					printf("error: inconsistent! send length: %d,  data length: %d\n", send_len, send_data.size());
			}

			if (send_len < 0) printf("send failed! error code: %d\n", WSAGetLastError());
			else
				printf("[%d] send succeeded     send len: %d\n", count, send_len);
			count += 1;
		}

		
	}
}

