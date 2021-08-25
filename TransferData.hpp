#ifndef __TRANSFERDATA_DATA_H
#define __TRANSFERDATA_DATA_H

#include "pugixml.hpp"
#include "DingDongData.hpp"

#define NEW_LINE(n) n + 1

constexpr uint32_t HEADER_TYPE = 4 + 2;
constexpr uint32_t HEADER_FROM = 4 + 2;
constexpr uint32_t HEADER_TO = 2 + 2;
constexpr uint32_t HEADER_PATH = 4 + 2;
constexpr uint32_t HEADER_LENGTH = 6 + 2;

constexpr uint32_t HEADER_TYPE_MAX = 1;
constexpr uint32_t HEADER_ID_MAX = 11;
constexpr uint32_t HEADER_PATH_MAX = USER_AVATAR_MAX;
constexpr uint32_t HEADER_LENGTH_MAX = 12;

// 普通请求头数据大小
constexpr uint32_t HEADER_MESSAGE_MAX = HEADER_TYPE + HEADER_TYPE_MAX + HEADER_FROM + HEADER_ID_MAX + HEADER_TO + HEADER_ID_MAX + HEADER_LENGTH + HEADER_LENGTH_MAX + NEW_LINE(4);
// 图片请求头数据大小
constexpr uint32_t HEADER_IMAGE_MAX = HEADER_TYPE + HEADER_TYPE_MAX + HEADER_FROM + HEADER_ID_MAX + HEADER_TO + HEADER_ID_MAX + HEADER_LENGTH + HEADER_LENGTH_MAX + HEADER_PATH + HEADER_PATH_MAX + NEW_LINE(5);


// 返回当前时间[uint64_t类型]
uint64_t retNowTime(const bool& is_day = false);

// 传输状态
enum TRANSFER_STATUS { request = 0, reply = 5 };
inline std::string transferStatusToStr(const TRANSFER_STATUS& type)
{
	if (type == TRANSFER_STATUS::request) return "request";
	if (type == TRANSFER_STATUS::reply) return "reply";
}
inline TRANSFER_STATUS strToTransferStatus(const std::string& str)
{
	if (str == "request") return TRANSFER_STATUS::request;
	if (str == "reply") return TRANSFER_STATUS::reply;
}

// 传输类型
enum TRANSFER_TYPE { _login = 0, _synchr = 5, _send = 10, _add = 15 };
inline std::string transferTypeToStr(const TRANSFER_TYPE& type)
{
	if (type == TRANSFER_TYPE::_login) return "login";
	if (type == TRANSFER_TYPE::_synchr) return "synchr";
	if (type == TRANSFER_TYPE::_send) return "send";
	if (type == TRANSFER_TYPE::_add) return "add";
}
inline TRANSFER_TYPE strToTransferType(const std::string& str)
{
	if (str == "login") return TRANSFER_TYPE::_login;
	if (str == "synchr") return TRANSFER_TYPE::_synchr;
	if (str == "send") return TRANSFER_TYPE::_send;
	if (str == "add") return TRANSFER_TYPE::_add;
}

// 解析请求头Type属性，判断时图片还是文本
// 返回第一个未读取的位置
const char * parseTypeHeader(uint32_t& type, const char* buff);

// 创建文本请求报文
/****************************************************************
 * 报文头组成部分
 * |Type:	| 发送类型 | 换行符
 * |From:   | 发送账号 | 换行符
 * |To:     | 接受账号 | 换行符
 * |Length: | 数据长度 | 换行符 × 2
 *
 * 对应的字节为：
 * | 6个字节 |  1个字节 | 1个字节
 * | 6个字节 | 11个字节 | 1个字节
 * | 4个字节 | 11个字节 | 1个字节
 * | 8个字节 | 12个字节 | 2个字节
 * 一共 64 个字节
 *
 * 空缺字节用'#'填充
 ****************************************************************/
std::string requestMessage(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, const ID& from, const ID& to, const std::string& t_data);
// 解析文本请求头的内容
// 返回第一个未读取的位置
const char * parseMessageHeader(std::string& from, std::string& to, uint64_t& len, const char* buff);

// 创建图片请求报文
/****************************************************************
 * 报文头组成部分
 * |Type:	| 发送类型 | 换行符
 * |From:   | 发送账号 | 换行符
 * |To:     | 接受账号 | 换行符
 * |Path:	| 图片路径 | 换行符
 * |Length: | 数据长度 | 换行符 × 2
 *
 * 对应的字节为：
 * | 6个字节 |   1个字节  | 1个字节
 * | 6个字节 |  11个字节  | 1个字节
 * | 4个字节 |  11个字节  | 1个字节
 * | 6个字节 | 120个字节  | 1个字节
 * | 8个字节 |  12个字节  | 2个字节
 * 一共 191 个字节
 *
 * 空缺字节用'#'填充
 ****************************************************************/
std::string requestImage(const char*  from, const char*  to, const char*  path);
// 解析文本请求头的内容
// 返回第一个未读取的位置
const char * parseImageHeader(std::string& from, std::string& to, std::string& path, uint64_t& len, const char* buff);

/****************************************************************
 * 登录请求头
 * from: 2248585019
 * to: 1
 * length: 32
 *
 * <data type="_login" status="request" from="client" to="server">
 *      <passward>123456<passward>
 * </data>
 *
 * 登录返回
 * from: 1
 * to: 2248585019
 * length: 29
 *
 * <data type="_login" status="reply" from="server" to="client">
 *      <passward>true<passward>
 * </data>
 ****************************************************************/

/****************************************************************
 * 同步请求
 * from: 2248585019
 * to: 1
 * length: 32
 *
 * <data type="_synchr" status="request" from="client" to="server">
 * </data>
 *
 * 同步返回
 * from: 1
 * to: 2248585019
 * length: 32
 *
 * <data type="_synchr" status="reply" from="server" to="client">
 *      <qq>...
 *      <qq>
 * </data>
 ****************************************************************/

/****************************************************************
 * 转发请求
 * From: 2248585019
 * To: 1961776643
 * Length: 39
 *
 * <data type="_send" status="request" from="user" to="user">
 *      <id>-1</id>
 *      <time>20210822213512</time>
 *      <message type="text">I'm so hungry</message>
 * </data>
 * OR
 * <data type="_send" status="request" from="user" to="group">
 *      <id>-1</id>
 *      <time>20210822213512</time>
 *      <message type="image">/image/message/123.jpg</message>
 * </data>
 *
 * 转发返回[返回ID]
 * from: 1
 * to: 2248585019
 * length: 25
 *
 * <data type="_send" status="reply" from="user" to="user">
 *      <id>4314</id>
 * </data>
 *
 ****************************************************************/
// 创建send请求类型的内容[XML]
std::string sendData(uint64_t time, const char * message, const MessageData::DATA_TYPE& m_type, const char * id = "-1");



/****************************************************************
 * 添加请求
 * from: 2248585019
 * to: 1961776643
 * length: 39
 *
 * <data type="add" status="request" from="user" to="user">
 *      <id>-1</id>
 *      <time>20210822213512</time>
 *      <verify>hi~ my name is Romeo</verify>
 * </data>
 * OR
 * <data type="add" status="request" from="user" to="group">
 *      <id>-1</id>
 *      <time>20210822213512</time>
 *      <verify>hi~</verify>
 * </data>
 *
 * 添加返回
 * from: 1
 * to: 2248585019
 * length: 25
 *
 * <data type="add" status="reply" from="server" to="client">
 *      <id>4314</id>
 * </data>
 *
 * 同意/拒绝 请求
 * from: 1961776643
 * to: 2248585019
 * length: 25
 *
 * <data type="add" status="reply" from="user" to="user">
 *      <id>4314</id>
 *      <time>20210822213512</time>
 *      <status>true</status>
 *      <verify>hi~ my name is Romeo</verify>
 * </data>
 * OR
 * <data type="add" status="reply" from="group" to="user">
 *      <id>4314</id>
 *      <time>20210822213512</time>
 *      <status>false</status>
 *      <verify>hi~</verify>
 * </data>
 ****************************************************************/

 // // 创建ID[XML格式]
 // inline std::string idData(const char * id) { return string("<id>") + id + "</id>"; };





#endif