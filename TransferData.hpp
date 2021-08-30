#ifndef __TRANSFERDATA_DATA_H
#define __TRANSFERDATA_DATA_H

#include "pugixml.hpp"
#include "DingDongData.hpp"

#define NEW_LINE(n) n + 1

constexpr uint32_t HEADER_KEY_TYPE = 4 + 2;				// 请求头：请求类型大小
constexpr uint32_t HEADER_KEY_FROM = 4 + 2;				// 请求头：发送方大小
constexpr uint32_t HEADER_KEY_TO = 2 + 2;				// 请求头：接受方大小
constexpr uint32_t HEADER_KEY_LENGTH = 6 + 2;			// 请求头：数据包长度大小

constexpr uint32_t HEADER_VALUE_TYPE = 1;					// 请求头：请求类型的值的大小
constexpr uint32_t HEADER_VALUE_ID = ID_MAX + 1;			// 请求头：发送和接受方的值的大小
constexpr uint32_t HEADER_VALUE_LENGTH = 12;				// 请求头：数据包长度的值的大小

// 请求头数据大小
constexpr uint32_t HEADER_TYPE_LENGTH = HEADER_KEY_TYPE + HEADER_VALUE_TYPE + 1;
// 普通请求头数据大小
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
	if (type == TRANSFER_STATUS::_reply) return "reply";
}
inline TRANSFER_STATUS strToTransferStatus(const std::string& str)
{
	if (str == "request") return TRANSFER_STATUS::_request;
	if (str == "reply") return TRANSFER_STATUS::_reply;
}

// 传输类型 { 注册，心跳包，登录，设置，发送，添加，删除 }
enum TRANSFER_TYPE { _register = 0, _heart = 2, _login = 4, _setting = 6, _send = 8, _add = 10, _delete = 12 };
inline std::string transferTypeToStr(const TRANSFER_TYPE& type)
{
	if (type == TRANSFER_TYPE::_register) return "register";
	if (type == TRANSFER_TYPE::_heart) return "heart";
	if (type == TRANSFER_TYPE::_login) return "login";
	if (type == TRANSFER_TYPE::_setting) return "setting";
	if (type == TRANSFER_TYPE::_send) return "send";
	if (type == TRANSFER_TYPE::_add) return "add";
	if (type == TRANSFER_TYPE::_delete) return "delete";
}
inline TRANSFER_TYPE strToTransferType(const std::string& str)
{
	if (str == "register") return TRANSFER_TYPE::_register;
	if (str == "heart") return TRANSFER_TYPE::_heart;
	if (str == "login") return TRANSFER_TYPE::_login;
	if (str == "setting") return TRANSFER_TYPE::_setting;
	if (str == "send") return TRANSFER_TYPE::_send;
	if (str == "add") return TRANSFER_TYPE::_add;
	if (str == "delete") return TRANSFER_TYPE::_delete;
}

class DDHeader
{


public:

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

	// 返回[请求头类型][禁止修改]
	inline const DDHEADER_TYPE retType(void) const { return _header_type; };
	// 设置[请求头类型]
	inline void setType(const DDHEADER_TYPE& type) { _header_type = type; };

	// 返回[发送方账号][禁止修改]
	inline ID retFromId(void) const { return _header_from; };
	// 设置[发送方账号]
	inline void setFromId(const ID& id) { _header_from = id; };

	// 返回[接收方账号][禁止修改]
	inline ID retToId(void) const { return _header_to; };
	// 设置[接收方账号]
	inline void setToId(const ID& id) { _header_to = id; };

	// 返回[请求头长度][禁止修改]
	inline const uint64_t retLength(void) const { return _header_length; };
	// 设置[请求头长度]
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
	// 解析[请求头类型的内容]
	const char * parseTypeHeader(const char* buff);
	// 解析[请求头属性的内容]
	const char * parseAttributesHeader(const char* buff);

	// 创建[文本数据]
	std::string createTextData(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, pugi::xml_document & node);
	// 处理[文本数据]
	void textDataHandler(const std::string& data);
	
	// 创建[图片数据]
	/****************************************************************
	 * 图片数据组成部分
	 * | 文件名称和后缀 | 图片操作[消息/头像] | 图片数据
	 * 第一个长度为：IMAGE_FILE_LENGTH
	 * 第二个长度为：1个字节
	 ****************************************************************/
	std::string createImageData(const char * path, const char * imagefile, const TRANSFER_TYPE& operate);
	// 处理[图片数据]
	void imageDataHandler(const std::string & data);

	// 创建[只有单独一个Id的XML数据]
	pugi::xml_document createOnlyID(const char *id);

	// 创建[注册XML数据]
	/****************************************************************
	 * 注册请求头
	 * from: 0
	 * to: 1
	 * length: 32
	 *
	 * <data type="register" status="request">
	 * 		<name>dog</name>
	 *      <passward>123456</passward>
	 * </data>
	 *
	 * 注册返回
	 * from: 1
	 * to: 0
	 * length: 29
	 *
	 * <data type="register" status="reply">
	 *      <uid>2248585019</uid>
	 * </data>
	 ****************************************************************/
	pugi::xml_document createRegisterDataRequest(const char *name, const char * passward);
	pugi::xml_document createRegisterDataReply(const char *id);
	// 处理程序[注册]
	void registeDataHandler(pugi::xml_node node, const TRANSFER_STATUS& status);

	/****************************************************************
	 * 心跳包请求头
	 * from: 2248585019
	 * to: 1
	 * length: 32
	 *
	 * <data type="heart" status="request"/>
	 *
	 * 心跳包无返回
	 ****************************************************************/
	// 处理程序[心跳包]
	void heartDataHandler(void);

	/****************************************************************
	 * 登录请求头
	 * from: 2248585019
	 * to: 1
	 * length: 32
	 *
	 * <data type="login" status="request">
	 *      <passward>123456</passward>
	 * </data>
	 *
	 * 登录返回
	 * from: 1
	 * to: 2248585019
	 * length: 29
	 *
	 * <data type="login" status="reply">
	 *      <passward>true</passward> or
	 *      <passward>false</passward>
	 * 		// passward为true则，包含七天以内的数据
	 * </data>
	 ****************************************************************/
	pugi::xml_document createLoginDataRequest(const char * passward);
	pugi::xml_document createLoginDataReply(const bool is_right);
	// 处理程序[登录]
	void loginDataHandler(pugi::xml_node node, const TRANSFER_STATUS& status);


	// 创建[发送XML数据]
	/****************************************************************
	 * 转发请求头
	 * from: 2248585019
	 * to: 1961776643
	 * length: 32
	 * 客户端send请求：
	 * <data type="send" status="request">
	 *      <id>0</id>
	 *		<time></time>
	 *		<message type="text"></message> or
	 *		<message type="image"></message>
	 * </data>
	 * 服务器send转发：
	 * <data type="send" status="request">
	 *      <id>111</id>
	 *		<time></time>
	 *		<message type="text"></message> or
	 *		<message type="image"></message>
	 * </data>
	 *
	 * 服务器的send主要是修改了ID,保持客户端和服务器的数据统一
	 *
	 * 服务器send返回
	 * from: 1
	 * to: 2248585019
	 * length: 29
	 *
	 * <data type="send" status="reply">
	 *      <id>111<id>
	 * </data>
	 ****************************************************************/
	pugi::xml_document createSendDataRequest(const uint64_t& id, const uint64_t& time, const MessageData::DATA_TYPE& m_type, const char * message);
	pugi::xml_document createSendDataReply(const char *id);
	// 处理程序[发送]
	void sendDataHandler(pugi::xml_node node, const TRANSFER_STATUS& status);

	// 创建[设置XML数据]
	/****************************************************************
	 * 设置请求头
	 * from: 2248585019
	 * to: 1
	 * length: 32
	 *
	 * <data type="setting" status="request">
	 *      <name>cat</name> or
	 *      <birth>20010713</birth> or
	 * 		<time>20210713</time> or
	 * 		<sex></sex> or
	 * 		...
	 * </data>
	 *
	 * 设置无返回
	 ****************************************************************/
	pugi::xml_document createSettingData(const char * key, const char * value);
	// 处理程序[设置]
	void settingDataHandler(pugi::xml_node node, const TRANSFER_STATUS& status);

	// 将一个文件复制到另一个路径下
	bool toOtherPath(const char * old_path, const char * new_path);

	// 设置文件到正确的路径下去
	bool setImageToRightPath(const char * oldpath, const TRANSFER_TYPE& operate, std::string& newpath, std::string& imagefile);

private:

	DDHEADER_TYPE _header_type;				// 请求头类型
	ID _header_from;						// 发送方账号
	ID _header_to;							// 接收方账号
	uint64_t _header_length;				// 请求头长度

	// 填充#符号
	std::string fillPoundKey(std::string &str, const uint32_t& max);

	// 创建[请求头属性][返回最后读取到位置]
	const char * parseAttributes(std::string & value, const uint32_t& header_name, const uint32_t& header_max, const char* buff);
	// 解析[请求头属性][返回字符串样式]
	std::string createAttributes(const std::string &key, const std::string & value, const uint32_t& header_max);

	// 创建[请求头ID的值的字符串格式]
	inline std::string createIdValueStr(const ID& id) const
	{
		if (id.retIdType() == ID::user) return std::string("u") + id.retId();
		if (id.retIdType() == ID::group) return std::string("g") + id.retId();
	}
	// 解析[请求头ID的值的字符串格式]
	inline ID::ID_TYPE parseIdValueStr(const char& ch)
	{
		if (ch == 'u') return ID::user;
		if (ch == 'g') return ID::group;
	}

	// 返回一个不重名的名字
	std::string giveAName(void);

	// 根据操作选择路径
	std::string choosePath(const TRANSFER_TYPE& operate);

	// 获取后缀
	inline std::string getSuffix(const std::string& file) { return file.substr(file.find_last_of('.') + 1); };


};

#ifdef _WIN32			// win平台

#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

typedef SOCKET dd_socket;

void ddRecv(dd_socket fd, char * buff, int len);

#endif

#ifdef __linux__			// Linux平台

typedef int dd_socket;

void ddRecv(dd_socket fd, char * buff, int len);

#endif




/****************************************************************
 * 登录请求头
 * from: 2248585019
 * to: 1
 * length: 32
 *
 * <data type="login" status="request">
 *      <passward>123456</passward>
 * </data>
 *
 * 登录返回
 * from: 1
 * to: 2248585019
 * length: 29
 *
 * <data type="login" status="reply">
 *      <passward>true</passward> or
 *      <passward>false</passward>
 * 		// passward为true则，包含七天以内的数据
 * </data>
 ****************************************************************/



#endif