#ifndef __DINGDONG_DATA_H
#define __DINGDONG_DATA_H

#include "pugixml.hpp"
#include <stdint.h>
#include <cstring>
#include <list>
#include <map>

constexpr uint32_t ID_MAX = 12;
constexpr uint32_t USER_NAME_MAX = 35;
constexpr uint32_t GROUP_NAME_MAX = 50;
constexpr uint32_t USER_AVATAR_MAX = 120;
constexpr uint32_t GROUP_AVATAR_MAX = 120;
constexpr uint32_t USER_SIGNATURE_MAX = 150;
constexpr uint32_t GROUP_INTRODUCTION_MAX = 255;
constexpr uint32_t MESSAGE_DATA_MAX = 600;
constexpr uint32_t VERIFY_MAX = 90;


// 用户数据
class UserData {

public:

    enum USER_SEX { null = 0, woman = 5, man = 10 };

    // 显示默认构造函数
    UserData(void) = default;

    // 列表初始化构造函数
    UserData(const char * id);

    // 列表初始化构造函数
    UserData(const char * id, const char * name, const USER_SEX& sex, const char * avatar, const uint32_t& birth, const char * signature);

    // 拷贝构造函数
    UserData(const UserData & other);

    // 赋值符号重载函数
    UserData & operator=(const UserData & other);

    // 显示默认析构函数
    ~UserData() = default;

    // 等于运算符重载函数
    bool operator==(const UserData & other) const;

    // 返回[账号][禁止修改]
    inline const char * retId(void) const { return _user_id; };

    // 返回[昵称][禁止修改]
    inline const char * retName(void) const { return _user_name; };
    // 设置[昵称]
    inline void setName(const char * name) { std::strncpy(_user_name, name, sizeof(_user_name)); };

    // 返回[性别][禁止修改]
    inline USER_SEX retSex(void) const { return _user_sex; };
    // 设置[性别]
    inline void setSex(const USER_SEX& sex) { _user_sex = sex; };

    // 返回[头像][禁止修改]
    inline const char * retAvatar(void) const { return _user_avatar; };
    // 设置[头像]
    inline void setAvatar(const char * avatar) { std::strncpy(_user_avatar, avatar, sizeof(_user_avatar)); };

    // 返回[生日][禁止修改]
    inline uint32_t retBirth(void) const { return _user_birth; };
    // 设置[生日]
    inline void setBirth(const uint32_t& birth) { _user_birth = birth; };

    // 返回[个性签名][禁止修改]
    inline const char * retSignature(void) const { return _user_signature; };
    // 设置[个性签名]
    inline void setSignature(const char * signature) { std::strncpy(_user_signature, signature, sizeof(_user_signature)); };

    // 转换成XML格式
    static std::string toXMLString(const UserData & user, int level = 0);

    // XML解析转换为UserData类型
    static UserData toUserData(const pugi::xml_node & node);

private:

    char _user_id[ID_MAX];                          // 账号
    char _user_name[USER_NAME_MAX];                 // 昵称
    USER_SEX _user_sex;                             // 性别
    char _user_avatar[USER_AVATAR_MAX];             // 头像
    uint32_t _user_birth;                           // 生日
    char _user_signature[USER_SIGNATURE_MAX];       // 个性签名

    // 设置[账号]
    inline void setId(const char * id) { std::strncpy(_user_id, id, sizeof(_user_id)); };

    // 根据字符串判断性别
    static inline USER_SEX judgingSex(const std::string& str) 
    { 
        if (str == "false") return USER_SEX::woman; 
        else if (str == "true") return USER_SEX::man; 
        else return USER_SEX::null; 
    };

    // 根据性别返回对应字符串
    static inline std::string sexConversionToString(const USER_SEX& sex)
    {
        if (sex == USER_SEX::woman) return "false";
        else if (sex == USER_SEX::man) return "true";
        else return "null";
    }

};


// 群数据
class GroupData {


public:

    // 显示默认构造函数
    GroupData(void) = default;

    // 列表初始化构造函数
    GroupData(const char * id);

    // 列表初始化构造函数
    GroupData(const char * id, const char * name, const char * avatar, const uint32_t& time, const char * introduction, const char * owner, const uint32_t& number);

    // 拷贝构造函数
    GroupData(const GroupData & group);

    // 赋值符号重载函数
    GroupData & operator=(const GroupData & group);

    // 显示默认析构函数
    ~GroupData() = default;

    // 等于运算符重载函数
    bool operator==(const GroupData & group) const;

    // 返回[群账号][禁止修改]
    inline const char * retId(void) const { return _group_id; };
    // 设置[群账号]
    inline void setId(const char * id) { std::strncpy(_group_id, id, sizeof(_group_id)); };

    // 返回[群名称][禁止修改]
    inline const char * retName(void) const { return _group_name; };
    // 设置[群名称]
    inline void setName(const char * name) { std::strncpy(_group_name, name, sizeof(_group_name)); };

    // 返回[群头像][禁止修改]
    inline const char * retAvatar(void) const { return _group_avatar; };
    // 设置[群头像]
    inline void setAvatar(const char * avatar) { std::strncpy(_group_avatar, avatar, sizeof(_group_avatar)); };

    // 返回[创建时间][禁止修改]
    inline uint32_t retTime(void) const{ return _group_time; };
    // 设置[创建时间]
    inline void setTime(const uint32_t& time) { _group_time = time; };

    // 返回[群介绍][禁止修改]
    inline const char * retIntroduction(void) const { return _group_introduction; };
    // 设置[群介绍]
    inline void setIntroduction(const char * introduction) { std::strncpy(_group_introduction, introduction, sizeof(_group_introduction)); };

    // 返回[群主][禁止修改]
    inline const char * retOwner(void) const { return _group_owner; };
    // 设置[群主]
    inline void setOwner(const char * owner) { std::strncpy(_group_owner, owner, sizeof(_group_owner)); };

    // 返回[群人数][禁止修改]
    inline uint32_t retNumber(void) const { return _group_number; };
    // 设置[群人数]
    inline void setNumber(const uint32_t& number) { _group_number = number; };

    // 转换成XML格式
    static std::string toXMLString(const GroupData & group, int level = 0);

    // XML解析转换为GroupData类型
    static GroupData toGroupData(const pugi::xml_node & node);

private:

    char _group_id[ID_MAX];                                 // 群账号
    char _group_name[GROUP_NAME_MAX];                       // 群名称
    char _group_avatar[GROUP_AVATAR_MAX];                   // 群头像
    uint32_t _group_time;                                   // 创建时间
    char _group_introduction[GROUP_INTRODUCTION_MAX];       // 群介绍
    char _group_owner[ID_MAX];                              // 群主
    uint32_t _group_number;                                 // 群人数

};


// 好友请求数据
class RequestData {


public:

    enum REQUEST_STATUS { untreated = 0, disagree = 5, agree = 10 };

    // 显示默认构造函数
    RequestData(void) = default;

    // 列表初始化构造函数
    RequestData(const uint32_t& id);

    // 列表初始化构造函数
    RequestData(const uint32_t& id, const char * from, const char * to, const char * verify, const REQUEST_STATUS& status, const uint32_t & time);

    // 拷贝构造函数
    RequestData(const RequestData & request);

    // 赋值符号重载函数
    RequestData & operator=(const RequestData & request);

    // 显示默认析构函数
    ~RequestData() = default;

    // 等于运算符重载函数
    bool operator==(const RequestData & request) const;

    // 返回[请求id][禁止修改]
    inline uint32_t retId(void) const { return _request_id; };
    // 设置[请求id]
    inline void setId(const uint32_t& id) { _request_id = id; };

    // 返回[发送id][禁止修改]
    inline const char * retFromId(void) const { return _from_id; };
    // 设置[发送id]
    inline void setFromId(const char * id) { std::strncpy(_from_id, id, sizeof(_from_id)); };

    // 返回[接收id][禁止修改]
    inline const char * retToId(void) const { return _to_id; };
    // 设置[接收id]
    inline void setToId(const char * id) { std::strncpy(_to_id, id, sizeof(_to_id)); };

    // 返回[验证信息][禁止修改]
    inline const char * retVerify(void) const { return _verify_message; };
    // 设置[验证信息]
    inline void setVerify(const char * verify) { std::strncpy(_verify_message, verify, sizeof(_verify_message)); };

    // 返回[是否处理][禁止修改]
    inline REQUEST_STATUS retStatus(void) const { return _request_status; };
    // 设置[是否处理]
    inline void setStatus(const REQUEST_STATUS& status) { _request_status = status; };

    // 返回[发送时间][禁止修改]
    inline uint32_t retTime(void) const { return _request_time; };
    // 设置[发送时间]
    inline void setTime(const uint32_t& time) { _request_time = time; };

    // 转换成XML格式
    static std::string toXMLString(const RequestData & request, int level = 0);

    // XML解析转换为RequestData类型
    static RequestData toRequestData(const pugi::xml_node & node);

private:

    uint32_t _request_id;               // 请求id
    char _from_id[ID_MAX];              // 发送id
    char _to_id[ID_MAX];                // 接收id
    char _verify_message[VERIFY_MAX];   // 验证信息
    REQUEST_STATUS _request_status;     // 是否处理
    uint32_t _request_time;             // 发送时间

    // 根据字符串判断是否处理
    static inline REQUEST_STATUS judgingProcess(const std::string& str) 
    { 
        if (str == "null") return REQUEST_STATUS::untreated; 
        else if (str == "true") return REQUEST_STATUS::agree; 
        else if (str == "false") return REQUEST_STATUS::disagree;
    };

    // 根据处理状态返回对应字符串
    static inline std::string processConversionToString(const REQUEST_STATUS& status)
    {
        if (status == REQUEST_STATUS::disagree) return "false";
        else if (status == REQUEST_STATUS::agree) return "true";
        else if (status == REQUEST_STATUS::untreated) return "null";
    }

};


// 消息数据
class MessageData {

public:
    
    // 显示默认构造函数
    MessageData(void) = default;

    // 列表初始化构造函数
    MessageData(const uint32_t & id);

    // 列表初始化构造函数
    MessageData(const uint32_t & id, const char * from, const char * to, const uint64_t time, const char * text);

    // 拷贝构造函数
    MessageData(const MessageData& message);

    // 赋值运算符重载函数
    MessageData & operator=(const MessageData& message);

    // 显示默认析构函数
    ~MessageData() = default;

    // 等于运算符重载函数
    bool operator==(const MessageData & message) const;

    // 返回[消息id][禁止修改]
    inline uint32_t retId(void) const { return _message_id; };
    // 设置[发送id]
    inline void setId(const uint32_t& id) { _message_id = id; };
    
    // 返回[发送账号][禁止修改]
    inline const char * retFromId(void) const { return _from_id; };
    // 设置[发送账号]
    inline void setFromId(const char * id) { strncpy(_from_id, id, sizeof(_from_id)); };

    // 返回[接受账号][禁止修改]
    inline const char * retToId(void) const { return _to_id; };
    // 设置[接受账号]
    inline void setToId(const char * id) { strncpy(_to_id, id, sizeof(_to_id)); };

    // 返回[发送时间][禁止修改]
    inline uint32_t retTime(void) const { return _send_time; };
    // 设置[发送时间]
    inline void setTime(const uint32_t& time) { _send_time = time; };

    // 返回[发送文本][禁止修改]
    inline const char * retText(void) const { return _message_text; };
    // 设置[发送文本]
    inline void setText(const char * text) { strncpy(_message_text, text, sizeof(_message_text)); };

    // 转换成XML格式
    static std::string toXMLString(const MessageData& message, int level = 0);

    // XML解析转换为UserData类型
    static MessageData toMessageData(const pugi::xml_node & node);

private:

    uint32_t _message_id;                           // 消息id
    char _from_id[ID_MAX];                          // 发送账号
    char _to_id[ID_MAX];                            // 接受账号
    uint64_t _send_time;                            // 发送时间
    char _message_text[MESSAGE_DATA_MAX];           // 发送文本

};


// 消息ID
struct MessageID {

    char _id[ID_MAX];
    enum ID_TYPE { user = 10, group = 20 }_type;

    // 列表初始化构造函数
    MessageID(const char *id, ID_TYPE type);

    // 小于运算符重载函数
    bool operator<(const MessageID& other) const;

};

// 好友列表
typedef std::list<UserData> UserList;

// 群列表
typedef std::list<GroupData> GroupList;

// 群成员
typedef std::map<std::string, UserList> GroupMemberList;

// 好友请求列表
typedef std::list<RequestData> RequestList;

// 消息列表
typedef std::map<MessageID, std::list<MessageData>> MessageList;

#ifdef __DINGDONG_DATA_DEBUG

#include <iostream>

#endif // __DINGDONG_DATA_DEBUG

// QQ数据
class DingDongData {



public:

    // 显示默认构造函数
    DingDongData(void) = default;
    
    // 列表初始化构造函数
    DingDongData(const UserData& user, const UserList& friends, const GroupList& groups, const GroupMemberList& members, const RequestList& requests, const MessageList & messages);
    
    // 拷贝构造函数
    DingDongData(const DingDongData& other);

    // 赋值运算符重载函数
    DingDongData & operator=(const DingDongData &other);

    // 显示默认析构函数
    ~DingDongData(void) = default;

    // 好友列表转换成XML格式
    static std::string friendListToXMLString(const UserList & friendlist, int level = 0);

    // XML解析转换为UserList类型
    static UserList toFriendList(const pugi::xml_node & node);

    // 群列表转换成XML格式
    static std::string groupListToXMLString(const GroupList & grouplist, int level = 0);
    
    // XML解析转换为UserList类型
    static GroupList toGroupList(const pugi::xml_node & node);

    // 群成员列表转换成XML格式
    static std::string groupMemberListToXMLString(const GroupMemberList & memberlist, int level = 0);

    // XML解析转换为UserList类型
    static GroupMemberList toGroupMemberList(const pugi::xml_node & node);

    // 消息列表转换成XML格式
    static std::string friendRequestListToXMLString(const RequestList & requestlist, int level = 0);
    
    // XML解析转换为UserList类型
    static RequestList toRequestList(const pugi::xml_node & node);

    // 消息列表转换成XML格式
    static std::string messageListToXMLString(const MessageList & messagelist, int level = 0);
    
    // XML解析转换为UserList类型
    static MessageList toMessageList(const pugi::xml_node & node);
    
    // 转换成XML格式
    static std::string toXMLString(const DingDongData & data, int level = 0);

    // XML解析转换为UserList类型
    static DingDongData toDingDongData(const pugi::xml_node & node);

    // 返回[个人信息][允许修改]
    inline UserData * retMyself(void) { return &_myself; };

    // 添加[好友]
    inline void addFriend(const UserData & other) { _friends.push_back(other); };
    // 删除[好友]
    inline void deleteFriend(const UserData & other) { _friends.remove(other); };
    // 返回[好友列表][禁止修改]
    inline const UserList * retFriend(void) const { return &_friends; };

    // 添加[群]
    inline void addGroup(const GroupData & other) { _groups.push_back(other); };
    // 删除[群]
    inline void deleteGroup(const GroupData & other) { _groups.remove(other); };
    // 返回[群列表][禁止修改]
    inline const GroupList * retGroup(void) const { return &_groups; };

    // 添加[群成员]
    inline void addGroupMember(const std::pair<std::string, UserList> & other) { _members.insert(other); };
    inline void addGroupMember(const std::string & group_id, const UserData & user) { _members[group_id].push_back(user); };
    // 删除[群成员]
    inline void deleteGroupMember(const std::string & group_id, const UserData & user) { _members[group_id].remove(user); };
    inline void deleteGroupMember(const std::string & group_id) { _members.erase(group_id); };
    // 返回[群列表][禁止修改]
    inline const GroupMemberList * retGroupMember(void) const { return &_members; };

    // 添加[好友请求]
    inline void addRequest(const RequestData & other) { _requests.push_back(other); };
    // 删除[好友请求]
    inline void deleteRequest(const RequestData & other) { _requests.remove(other); };
    // 返回[好友请求列表][禁止修改]
    inline const RequestList * retRequest(void) const { return &_requests; };

    // 添加[消息]
    inline void addMessage(const std::pair<MessageID, std::list<MessageData>> & other) { _messages.insert(other); };
    inline void addMessage(const MessageID & id, const MessageData & message) { _messages[id].push_back(message); };
    // 删除[消息]
    inline void deleteMessage(const MessageID & id, const MessageData & message) { _messages[id].remove(message); };
    inline void deleteMessage(const MessageID  & id) { _messages.erase(id); };
    // 返回[消息列表][禁止修改]
    inline const MessageList * retMessage(void) const { return &_messages; };

#ifdef __DINGDONG_DATA_DEBUG

    void show(void) const
    {
        // 用户
        std::cout << "myself: " << std::string(_myself.retId()) << std::endl;

        // 好友
        std::cout << "friends: \n";
        for (auto a : _friends)
            std::cout << "\tid: " << std::string(a.retId()) << std::endl;

        // 群
        std::cout << "groups: \n";
        for (auto a : _groups)
            std::cout << "\tid: " << std::string(a.retId()) << std::endl;
        
        // 群成员
        std::cout << "members: \n";
        for (auto a : _members)
        {
            std::cout << "\tgroup id: " << a.first << std::endl;
            for (auto b : a.second)
                std::cout << "\t\t user id: " << std::string(b.retId()) << std::endl;
        }

        // 请求
        std::cout << "requests: \n";
        for (auto a : _requests)
            std::cout << "\tid: " << std::to_string(a.retId()) << std::endl;
        
        // 消息
        std::cout << "messages: \n";
        for (auto a : _messages)
        {
            if (a.first._type == MessageID::user)
                std::cout << "\tuser id: ";
            else if (a.first._type == MessageID::group)
                std::cout << "\tgroup id: ";

            std::cout << std::string(a.first._id) << std::endl;
            
            for (auto b : a.second)
                std::cout << "\t\t message id: " << std::to_string(b.retId()) << std::endl;
        }
        std::cout << "\n\n\n\n\n" ;
    }

#endif // __DINGDONG_DATA_DEBUG

private:

    UserData _myself;                       // 个人信息
    UserList _friends;                      // 好友列表
    GroupList _groups;                      // 群列表
    GroupMemberList _members;               // 群成员
    RequestList _requests;                  // 好友请求
    MessageList _messages;                  // 消息列表

};

#endif // __DINGDONG_DATA_H