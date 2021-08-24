#include "TransferData.hpp"
#include <chrono>
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

string SendData(uint64_t time, const char * message, const MessageData::DATA_TYPE& m_type, const char * id)
{
    string xml;

    xml += "\t<id>" + string(id) + "</id>\n";
    xml += "\t<time>" + to_string(time) + "</time>\n";
    xml += "\t<message type=\"" + MessageData::dataTypeToStr(m_type) + to_string(time) + "</message>\n";

    return xml;
}

string requestMessage(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, const ID& from, const ID& to, const string& type_data)
{
    string header, data;

    header += "From: " + string(from.retId());
    for (int i = Header_ID_MAX - strlen(from.retId()); i > 0; --i)
        header += "#";
    header += "\n";

    header += "To: " + string(to.retId());
    for (int i = Header_ID_MAX - strlen(to.retId()); i > 0; --i)
        header += "#";
    header += "\n";

    data += "<data type=\"" + transferTypeToStr(type);
    data += "\" status=\"" + transferStatusToStr(status);
    data += "\" from=\"" + transferStatusToStr(status);
    data += "\" group=\"" + transferStatusToStr(status) + "\">\n";
    data += type_data;
    data += "</data>\n";

	header += "Length: " + to_string(data.size());
	for (int i = Header_LENGTH_MAX - to_string(data.size()).size(); i > 0; --i)
		header += "#";
	header += "\n\n";

    return header + data;
}

const char * ParseMessageHeader(std::string&from, std::string&to, uint64_t& len, const char* buff)
{
	from = to = "";
	const char * temp = buff;

	// 读取from
	for (const char * p = temp + Header_FROM; p != (temp + Header_FROM + Header_ID_MAX); ++p)
	{
		if (*p != '#')
			from.push_back(*p);
	}
	temp += Header_FROM + Header_ID_MAX + 1;

	// 读取to
	for (const char * p = temp + Header_TO; p != (temp + Header_TO + Header_ID_MAX); ++p)
	{
		if (*p != '#')
			to.push_back(*p);
	}
	temp += Header_TO + Header_ID_MAX + 1;

	// 读取length
	string len_str;
	for (const char * p = temp + Header_LENGTH; p != (temp + Header_LENGTH + Header_LENGTH_MAX); ++p)
	{
		if (*p != '#')
			len_str.push_back(*p);
	}
	len = std::stoll(len_str.c_str());
	temp += Header_LENGTH + Header_LENGTH_MAX + 2;

	return temp;
}

