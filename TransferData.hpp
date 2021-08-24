#ifndef __TRANSFERDATA_DATA_H
#define __TRANSFERDATA_DATA_H

#include "pugixml.hpp"
#include "DingDongData.hpp"

constexpr uint32_t Header_FROM = 6;
constexpr uint32_t Header_TO = 4;
constexpr uint32_t Header_LENGTH = 8;
constexpr uint32_t Header_ID_MAX = 11;
constexpr uint32_t Header_LENGTH_MAX = 12;
constexpr uint32_t Header_MAX = 56;

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
enum TRANSFER_TYPE { login = 0, synchr = 5, send = 10, add = 15 };
inline std::string transferTypeToStr(const TRANSFER_TYPE& type)
{
    if (type == TRANSFER_TYPE::login) return "login";
    if (type == TRANSFER_TYPE::synchr) return "synchr";
    if (type == TRANSFER_TYPE::send) return "send";
    if (type == TRANSFER_TYPE::add) return "add";
}
inline TRANSFER_TYPE strToTransferType(const std::string& str)
{
    if (str == "login") return TRANSFER_TYPE::login;
    if (str == "synchr") return TRANSFER_TYPE::synchr;
    if (str == "send") return TRANSFER_TYPE::send;
    if (str == "add") return TRANSFER_TYPE::add;
}



// 创建请求报文
/****************************************************************
 * 报文头组成部分
 * |From:   | 发送账号 | 换行符
 * |To:     | 接受账号 | 换行符
 * |Length: | 数据长度 | 换行符 × 2
 * 
 * 对应的字节为：
 * | 6个字节 | 11个字节 | 1个字节
 * | 4个字节 | 11个字节 | 1个字节
 * | 8个字节 | 12个字节 | 2个字节
 * 一共 56 个字节
 * 
 * 空缺字节用'#'填充
 ****************************************************************/
std::string requestMessage(const TRANSFER_TYPE& type, const TRANSFER_STATUS& status, const ID& from, const ID& to, const std::string& type_data);

// 解析报文头
const char * ParseMessageHeader(std::string&from, std::string&to, uint64_t& len, const char* buff);

/****************************************************************
 * 登录请求头
 * from: 2248585019
 * to: 1
 * length: 32
 * 
 * <data type="login" status="request" from="client" to="server">
 *      <passward>123456<passward>
 * </data>
 * 
 * 登录返回
 * from: 1
 * to: 2248585019
 * length: 29
 * 
 * <data type="login" status="reply" from="server" to="client">
 *      <passward>true<passward>
 * </data>
 ****************************************************************/

/****************************************************************
 * 同步请求
 * from: 2248585019
 * to: 1
 * length: 32
 * 
 * <data type="synchr" status="request" from="client" to="server">
 * </data>
 * 
 * 同步返回
 * from: 1
 * to: 2248585019
 * length: 32
 * 
 * <data type="synchr" status="reply" from="server" to="client">
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
 * <data type="send" status="request" from="user" to="user">
 *      <id>-1</id>
 *      <time>20210822213512</time>
 *      <message type="text">I'm so hungry</message>
 * </data>
 * OR
 * <data type="send" status="request" from="user" to="group">
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
 * <data type="send" status="reply" from="user" to="user">
 *      <id>4314</id>
 * </data>
 *
 ****************************************************************/
// 创建Send请求类型的内容[XML]
std::string sendData(uint64_t time, const char * message, const MessageData::DATA_TYPE& type, const char * id = "-1");



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