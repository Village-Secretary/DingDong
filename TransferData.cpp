#include "TransferData.hpp"
#include <chrono>
#include <fstream>
#include <sstream>

using std::string;
using std::stringstream;
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

	// 读取Type
	string type_str;
	temp = retAttributes(type_str, HEADER_TYPE, HEADER_TYPE_MAX, temp);
	type = std::stoll(type_str.c_str());

	return temp;
}

string requestMessage(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, const ID& from, const ID& to, const string& t_data)
{
	string header, data;

	header += setAttributes("Type", "1", HEADER_TYPE_MAX) + "\n";
	header += setAttributes("From", from.retId(), HEADER_ID_MAX) + "\n";
	header += setAttributes("To", to.retId(), HEADER_ID_MAX) + "\n";

	data += "<data type=\"" + transferTypeToStr(type);
	data += "\" status=\"" + transferStatusToStr(status);
	data += "\" from=\"" + transferStatusToStr(status);
	data += "\" group=\"" + transferStatusToStr(status) + "\">\n";
	data += t_data;
	data += "</data>\n";

	header += setAttributes("Length", to_string(data.size()), HEADER_LENGTH_MAX) + "\n";
	header += "\n";

	return header + data;
}
const char * parseMessageHeader(std::string&from, std::string&to, uint64_t& len, const char* buff)
{
	from = to = "";
	const char * temp = buff;

	temp = retAttributes(from, HEADER_FROM, HEADER_ID_MAX, temp);
	temp = retAttributes(to, HEADER_TO, HEADER_ID_MAX, temp);

	string len_str;
	temp = retAttributes(len_str, HEADER_LENGTH, HEADER_LENGTH_MAX, temp);
	len = std::stoll(len_str.c_str());

	temp += 1;

	return temp;
}

string requestImage(const char*  from, const char*  to, const char*  path)
{
	string header, data;

	header += setAttributes("Type", "2", HEADER_TYPE_MAX) + "\n";
	header += setAttributes("From", from, HEADER_ID_MAX) + "\n";
	header += setAttributes("To", to, HEADER_ID_MAX) + "\n";
	header += setAttributes("Path", path, HEADER_PATH_MAX) + "\n";

	// 读取图片
	data.data();

	header += setAttributes("Length", to_string(data.size()), HEADER_LENGTH_MAX) + "\n";
	header += "\n";

	return header + data;
}
const char * parseImageHeader(std::string& from, std::string& to, std::string& path, uint64_t& len, const char* buff)
{
	from = to = path = "";
	const char * temp = buff;

	temp = retAttributes(from, HEADER_FROM, HEADER_ID_MAX, temp);
	temp = retAttributes(to, HEADER_TO, HEADER_ID_MAX, temp);
	temp = retAttributes(path, HEADER_PATH, HEADER_PATH_MAX, temp);

	string len_str;
	temp = retAttributes(len_str, HEADER_LENGTH, HEADER_LENGTH_MAX, temp);
	len = std::stoll(len_str.c_str());

	temp += 1;

	return temp;
}