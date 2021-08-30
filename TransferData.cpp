#define __DD_SERVER
//#define __DD_CLIENT

#include "TransferData.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::to_string;

struct xml_string_writer : pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result.append(static_cast<const char*>(data), size);
	}
};

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

string DDHeader::fillPoundKey(string &str, const uint32_t& max)
{
	for (int i = max; i > 0; --i)
		str += "#";
	return str;
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
	attributes += key + ": " + value;

	return fillPoundKey(attributes, header_max - value.size());
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

string DDHeader::createTextData(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, pugi::xml_document & xml_doc)
{
	pugi::xml_document doc;
	pugi::xml_node pre = doc.prepend_child(pugi::node_declaration);	
	pre.append_attribute("version") = "1.0";
	pre.append_attribute("encoding") = "utf-8";

	// 添加节点[data]
	pugi::xml_node root = doc.append_copy(xml_doc.child("data"));
	
	// 添加属性
	root.append_attribute("type") = transferTypeToStr(type).c_str();
	root.append_attribute("status") = transferStatusToStr(status).c_str();

	// 转换成string类型
	xml_string_writer data;
	doc.save(data);

	return data.result;
}
void DDHeader::textDataHandler(const std::string & data)
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
	TRANSFER_TYPE type = strToTransferType(node_data.attribute("type").value());
	TRANSFER_STATUS status = strToTransferStatus(node_data.attribute("status").value());

	// 根据数据包类型和状态做出相应处理
	switch (type)
	{
	case _login:
		break;
	// case _synchr:
	// 	break;
	case _send:
		break;
	case _add:
		break;
	default:
		break;
	}

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
	s_imagefile = fillPoundKey(s_imagefile, IMAGE_FILE_LENGTH - s_imagefile.size());
	s_type = fillPoundKey(s_type, TRANSFER_TYPE_STRING_LENGTH - s_type.size());
	

	// 如果文件名和后缀大于IMAGE_FILE_LENGTH，则创建失败
	if (s_imagefile.size() > IMAGE_FILE_LENGTH)
	{
		std::cout << "abnormal: IMAGE_FILE_LENGTH![createImageData]" << std::endl;
		return "";
	}

	return s_imagefile + s_type + data;
}
void DDHeader::imageDataHandler(const std::string & data)
{
	if (data.size() == 0) return;

	string path, s_imagefile, s_type;

	// 获取图片后缀和图片操作
	parseAttributes(s_type, 0, TRANSFER_TYPE_STRING_LENGTH, parseAttributes(s_imagefile, 0, IMAGE_FILE_LENGTH, data.data()));

	path = choosePath(strToTransferType(s_type));		// 根据图片操作选择路径

	// 创建存储改图片
	ofstream image_file(path + s_imagefile, ofstream::binary);
	if (!image_file)
	{
		std::cout << "abnormal: ofstream![imageDataHandler]" << std::endl;
		return;
	}
	
	// 写入数据
	for (auto p = data.begin() + IMAGE_FILE_LENGTH + TRANSFER_TYPE_STRING_LENGTH; p != data.end(); ++p)
		image_file.put(*p);
	image_file.close();

	// 存储/修改数据库数据

	// 如果是服务器且operate为0：返回一个ID

	// 如果是服务器：转发此图片数据

}

pugi::xml_document createOnlyID(const char *id)
{
	pugi::xml_document doc;
	pugi::xml_node data = doc.append_child("data");
	
	pugi::xml_node id_node = data.append_child("id");

	id_node.append_child(pugi::node_pcdata).set_value(id);

	return doc;
}

pugi::xml_document DDHeader::createRegisterDataRequest(const char *name, const char * passward)
{
	pugi::xml_document doc;
	pugi::xml_node data = doc.append_child("data");
	
	pugi::xml_node name_node = data.append_child("name");
	pugi::xml_node passward_node = data.append_child("passward");

	name_node.append_child(pugi::node_pcdata).set_value(name);
	passward_node.append_child(pugi::node_pcdata).set_value(passward);

	return doc;
}
pugi::xml_document DDHeader::createRegisterDataReply(const char *id)
{
	return createOnlyID(id);
}

pugi::xml_document createLoginDataRequest(const char * passward)
{

}
pugi::xml_document createLoginDataReply(const bool is_right)
{
	
}

pugi::xml_document DDHeader::createSendDataRequest(const uint64_t & id, const uint64_t & time, const MessageData::DATA_TYPE & m_type, const char * message)
{
	pugi::xml_document doc;
	pugi::xml_node data = doc.append_child("data");
	
	pugi::xml_node id_node = data.append_child("id");
	pugi::xml_node time_node = data.append_child("time");

	id_node.append_child(pugi::node_pcdata).set_value(to_string(id).c_str());
	time_node.append_child(pugi::node_pcdata).set_value(to_string(time).c_str());

	pugi::xml_node node_message = data.append_child("message");
	node_message.append_attribute("type") = MessageData::dataTypeToStr(m_type).c_str();
	node_message.append_child(pugi::node_pcdata).set_value(message);

	return doc;
}
pugi::xml_document DDHeader::createSendDataReply(const char *id)
{
	return createOnlyID(id);
}

pugi::xml_document DDHeader::createSettingData(const char * key, const char * value)
{
	pugi::xml_document doc;
	pugi::xml_node data = doc.append_child("data");
	
	pugi::xml_node label = data.append_child(key);
	label.append_child(pugi::node_pcdata).set_value(value);

	return doc;
}



string DDHeader::giveAName(void)
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

string DDHeader::choosePath(const TRANSFER_TYPE& operate)
{
	switch (operate)
	{
	case TRANSFER_TYPE::_send: 
		if (_header_to.retIdType() == ID::user) return IMAGE_USER_MESSAGE;
		if (_header_to.retIdType() == ID::group) return IMAGE_GROUP_MESSAGE;
		break;
	case TRANSFER_TYPE::_setting:
		if (_header_to.retIdType() == ID::user) return IMAGE_USER_AVATAR;
		if (_header_to.retIdType() == ID::group) return IMAGE_GROUP_AVATAR;
		break;
	}
}

bool DDHeader::toOtherPath(const char * old_path, const char * new_path)
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
	for (auto p = image_data.begin(); p != image_data.end(); ++p)
		file_out.put(*p);
	file_out.close();

	return true;
}

bool DDHeader::setImageToRightPath(const char * oldpath, const TRANSFER_TYPE& operate, string& newpath, string& imagefile)
{
	newpath = choosePath(operate);				// 获取路径
	imagefile = giveAName();					// 获取名字
	imagefile += "." + getSuffix(oldpath);		// 获取后缀
	
	if (toOtherPath(oldpath, (newpath + imagefile).c_str()))	// 移动文件
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
// 接受数据
void ddRecv(dd_socket fd, char * buff, int len)
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
				data_buff += string(p, recv_end - p);		// 暂存数据
				data_len_count -= (recv_end - p);			// 记录还需读取的数据大小

				break;
			}
			else
			{
				data_buff += string(p, data_len_count);

				if (dd_header.retType() == DDHeader::message) dd_header.textDataHandler(data_buff);
				if (dd_header.retType() == DDHeader::image) dd_header.imageDataHandler(data_buff);

				p += data_len_count;			// 调整读取位置p
				data_buff.clear();				// 清理数据缓存
				data_len_count = 0;				// 数据包计数器清零
				isType = true;
			}

		}
	}

	if (recv_len < 0) std::cout << "recv failed! error code: " << WSAGetLastError() << std::endl;
}
