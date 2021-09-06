#ifndef __TRANSFERDATA_DATA_H
#define __TRANSFERDATA_DATA_H

#include "pugixml.hpp"
#include "DingDongData.hpp"
#include <Queue>

#define NEW_LINE(n) n + 1


constexpr uint32_t HEADER_KEY_TYPE = 4 + 2;				// 报文头：请求类型大小
constexpr uint32_t HEADER_KEY_FROM = 4 + 2;				// 报文头：发送方大小
constexpr uint32_t HEADER_KEY_TO = 2 + 2;				// 报文头：接受方大小
constexpr uint32_t HEADER_KEY_LENGTH = 6 + 2;			// 报文头：数据包长度大小

constexpr uint32_t HEADER_VALUE_TYPE = 1;					// 报文头：请求类型的值的大小
constexpr uint32_t HEADER_VALUE_ID = ID_LENGTH + 1;			// 报文头：发送和接受方的值的大小（注： +1 是因为要比普通账号多一个字符，为u或g，表示用户或群）
constexpr uint32_t HEADER_VALUE_LENGTH = 12;				// 报文头：数据包长度的值的大小

// 报文头数据大小
constexpr uint32_t HEADER_TYPE_LENGTH = HEADER_KEY_TYPE + HEADER_VALUE_TYPE + 1;
// 普通报文头数据大小
constexpr uint32_t HEADER_ATTRIBUTES_LENGTH = HEADER_KEY_FROM + HEADER_VALUE_ID + HEADER_KEY_TO + HEADER_VALUE_ID + HEADER_KEY_LENGTH + HEADER_VALUE_LENGTH + NEW_LINE(3);

// 图片名称大小
constexpr uint32_t IMAGE_FILE_LENGTH = 30;
// 传输类型字符串长度
constexpr uint32_t TRANSFER_TYPE_STRING_LENGTH = 15;

// 图片路径
#ifdef __DD_CLIENT

constexpr const char * IMAGE_ROOT_DIRECTORY = "client\\image\\";
constexpr const char * IMAGE_USER_AVATAR = "client\\image\\user_avatar\\";
constexpr const char * IMAGE_GROUP_AVATAR = "client\\image\\group_avatar\\";
constexpr const char * IMAGE_USER_MESSAGE = "client\\image\\user_message\\";
constexpr const char * IMAGE_GROUP_MESSAGE = "client\\image\\group_message\\";

#endif

#ifdef __DD_SERVER

constexpr const char * IMAGE_ROOT_DIRECTORY = "server\\image\\";
constexpr const char * IMAGE_USER_AVATAR = "server\\image\\user_avatar\\";
constexpr const char * IMAGE_GROUP_AVATAR = "server\\image\\group_avatar\\";
constexpr const char * IMAGE_USER_MESSAGE = "server\\image\\user_message\\";
constexpr const char * IMAGE_GROUP_MESSAGE = "server\\image\\group_message\\";

#endif

// 返回当前时间[uint64_t类型]
uint64_t retNowTime(const bool& is_day = false);

// 传输状态
enum TRANSFER_STATUS { _request = 0, _reply = 5 };
inline std::string transferStatusToStr(const TRANSFER_STATUS& type)
{
	if (type == TRANSFER_STATUS::_request) return "request";
	else if (type == TRANSFER_STATUS::_reply) return "reply";
}
inline TRANSFER_STATUS strToTransferStatus(const std::string& str)
{
	if (str == "request") return TRANSFER_STATUS::_request;
	else if (str == "reply") return TRANSFER_STATUS::_reply;
}

// 传输类型 { 注册，心跳包，登录，设置，发送，添加，删除, 请求 }
enum TRANSFER_TYPE { _register = 0, _heart = 2, _login = 4, _setting = 6, _send = 8, _add = 10, _delete = 12, t_request = 14 };
inline std::string transferTypeToStr(const TRANSFER_TYPE& type)
{
	if (type == TRANSFER_TYPE::_register) return "register";
	if (type == TRANSFER_TYPE::_heart) return "heart";
	if (type == TRANSFER_TYPE::_login) return "login";
	if (type == TRANSFER_TYPE::_setting) return "setting";
	if (type == TRANSFER_TYPE::_send) return "send";
	if (type == TRANSFER_TYPE::_add) return "add";
	if (type == TRANSFER_TYPE::_delete) return "delete";
	if (type == TRANSFER_TYPE::t_request) return "request";
}
inline TRANSFER_TYPE strToTransferType(const std::string& str)
{
	if (str == "register") return TRANSFER_TYPE::_register;
	else if (str == "heart") return TRANSFER_TYPE::_heart;
	else if (str == "login") return TRANSFER_TYPE::_login;
	else if (str == "setting") return TRANSFER_TYPE::_setting;
	else if (str == "send") return TRANSFER_TYPE::_send;
	else if (str == "add") return TRANSFER_TYPE::_add;
	else if (str == "delete") return TRANSFER_TYPE::_delete;
	else if (str == "request") return TRANSFER_TYPE::t_request;
}


// 报文头
class DDHeader
{


public:

	// 报文头类型： 信息，图片
	enum DDHEADER_TYPE { message = 0, image = 1 };

	// 显示默认构造函数
	DDHeader(void) = default;

	// 列表初始化构造函数
	DDHeader(const DDHEADER_TYPE& type, const ID& from, const ID& to, const uint64_t& len = 0);

	// 拷贝构造函数
	DDHeader(const DDHeader& other);

	// 赋值运算符重载函数
	DDHeader & operator=(const DDHeader& other);

	// 显示默认析构函数
	virtual ~DDHeader(void) = default;

	// 返回[报文头类型][禁止修改]
	inline const DDHEADER_TYPE retType(void) const { return _header_type; };
	// 设置[报文头类型]
	inline void setType(const DDHEADER_TYPE& type) { _header_type = type; };

	// 返回[发送方账号][禁止修改]
	inline ID retFromId(void) const { return _header_from; };
	// 设置[发送方账号]
	inline void setFromId(const ID& id) { _header_from = id; };

	// 返回[接收方账号][禁止修改]
	inline ID retToId(void) const { return _header_to; };
	// 设置[接收方账号]
	inline void setToId(const ID& id) { _header_to = id; };

	// 返回[报文头长度][禁止修改]
	inline const uint64_t retLength(void) const { return _header_length; };
	// 设置[报文头长度]
	inline void setLength(const uint64_t& len) { _header_length = len; };

	// 创建[文本请求报文]
	/****************************************************************
	 * 报文头组成部分
	 * |Type:	| 发送类型 | 换行符
	 * |From:   | 发送账号 | 换行符
	 * |To:     | 接受账号 | 换行符
	 * |Length: | 数据长度 | 换行符 × 2
	 *
	 * 对应的字节为：
	 * | 6个字节 |  1个字节 | 1个字节
	 * | 6个字节 | 13个字节 | 1个字节
	 * | 4个字节 | 13个字节 | 1个字节
	 * | 8个字节 | 12个字节 | 2个字节
	 * 一共 68 个字节
	 *
	 * 空缺字节用'#'填充
	 ****************************************************************/
	std::string createHeader(const std::string& t_data);
	// 解析[报文头类型的内容]
	const char * parseTypeHeader(const char* buff);
	// 解析[报文头属性的内容]
	const char * parseAttributesHeader(const char* buff);

	// 创建[文本数据]
	template<typename T>
	std::string createTextData(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, const T & data)
	{
		pugi::xml_document doc;
		pugi::xml_node pre = doc.prepend_child(pugi::node_declaration);
		pre.append_attribute("version") = "1.0";
		pre.append_attribute("encoding") = "utf-8";

		pugi::xml_node root = doc.append_child("data");
		// 添加属性
		root.append_attribute("type") = transferTypeToStr(type).c_str();
		root.append_attribute("status") = transferStatusToStr(status).c_str();

		data.toXML(data, root);

		return xmlToString(doc);
	}
	void textDataHandler(const std::string & data, TRANSFER_TYPE& type, TRANSFER_STATUS& status);

	// 创建[图片数据]
	/****************************************************************
	 * 图片数据组成部分
	 * | 文件名称和后缀 | 图片操作[消息/头像] | 图片数据
	 * 第一个长度为：IMAGE_FILE_LENGTH
	 * 第二个长度为：1个字节
	 ****************************************************************/
	std::string createImageData(const char * path, const char * imagefile, const TRANSFER_TYPE& operate);
	// 下载[图片数据]
	void imageDataHandler(const std::string & data, std::string& path, std::string& imagefile, TRANSFER_TYPE& operate);

private:

	DDHEADER_TYPE _header_type;				// 报文头类型
	ID _header_from;						// 发送方账号
	ID _header_to;							// 接收方账号
	uint64_t _header_length;				// 报文头长度

	// 填充#符号
	void fillPoundKey(std::string &data, const uint32_t& max);
	const char * findDataInPoundKey(std::string & data, const uint32_t& bg, const uint32_t& ed, const char* buff);

	// 创建[报文头ID的值的字符串格式]
	inline std::string createIdValueStr(const ID& id) const
	{
		if (id.retIdType() == ID::user) return std::string("u") + id.retId();
		if (id.retIdType() == ID::group) return std::string("g") + id.retId();
	}
	// 解析[报文头ID的值的字符串格式]
	inline ID::ID_TYPE parseIdValueStr(const char& ch)
	{
		if (ch == 'u') return ID::user;
		if (ch == 'g') return ID::group;
	}

};


// 返回一个不重名的名字
std::string giveAName(void);

// 根据操作选择路径
std::string choosePath(const DDHeader& header, const TRANSFER_TYPE& operate);

// 获取后缀
std::string getSuffix(const std::string& file);

// 将一个文件复制到另一个路径下
bool toOtherPath(const char * old_path, const char * new_path);

// 设置文件到正确的路径下去
bool setImageToRightPath(const DDHeader& header, const char * oldpath, const TRANSFER_TYPE& operate, std::string& newpath, std::string& imagefile);



#ifdef _WIN32			// win平台

#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

typedef SOCKET dd_socket;

void ddRecv(dd_socket fd, char * buff, int len);

void ddSend(dd_socket fd);

#endif

#ifdef __linux__			// Linux平台

typedef int dd_socket;

void ddRecv(dd_socket fd, char * buff, int len);

#endif



#endif