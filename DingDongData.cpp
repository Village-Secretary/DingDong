#include "DingDongData.hpp"
#include <string>
#include <algorithm>

using std::string;
using std::to_string;
using std::stoi;
using std::stoll;
using pugi::xml_node;
using std::list;
using std::make_pair;


// 列表初始化构造函数
ID::ID(const char *id, ID_TYPE type) : _type(type)
{
	strncpy(_id, id, sizeof(_id));
}

ID::ID(const ID& other) : _type(other._type)
{
	strncpy(_id, other._id, sizeof(_id));
}

// 赋值符号重载函数
ID & ID::operator=(const ID & other)
{
	strncpy(_id, other._id, sizeof(_id));
	_type = other._type;

	return *this;
}

// 等于运算符重载函数
bool ID::operator==(const ID& other) const
{
	if ((strcmp(_id, other._id) == 0) && (_type == other._type))
		return true;
	return false;
}

// 比较运算符重载函数
bool ID::operator<(const ID& other) const
{
	if (strcmp(_id, other._id) < 0)
	{
		return true;
	}
	else if (strcmp(_id, other._id) == 0)
	{
		if (_type == other._type) return false;
		else if (_type == ID::ID_TYPE::user) return true;
		else if (_type == ID::ID_TYPE::group) return false;
	}
	return false;
}

// 列表初始化构造函数
UserData::UserData(const char * id, const char * name, const USER_SEX& sex, const char * avatar, const uint32_t& birth, const char * signature) : _user_sex(sex), _user_birth(birth)
{
	strncpy(_user_id, id, sizeof(_user_id));
	strncpy(_user_name, name, sizeof(_user_name));
	strncpy(_user_avatar, avatar, sizeof(_user_avatar));
	strncpy(_user_signature, signature, sizeof(_user_signature));
}

// 列表初始化构造函数
UserData::UserData(const char * id) { strncpy(_user_id, id, sizeof(_user_id)); };

// 拷贝构造函数
UserData::UserData(const UserData & other) : _user_sex(other._user_sex), _user_birth(other._user_birth)
{
	strncpy(_user_id, other._user_id, sizeof(_user_id));
	strncpy(_user_name, other._user_name, sizeof(_user_name));
	strncpy(_user_avatar, other._user_avatar, sizeof(_user_avatar));
	strncpy(_user_signature, other._user_signature, sizeof(_user_signature));
}

// 赋值符号重载函数
UserData & UserData::operator=(const UserData & other)
{
	strncpy(_user_id, other._user_id, sizeof(_user_id));
	strncpy(_user_name, other._user_name, sizeof(_user_name));
	_user_sex = other._user_sex;
	strncpy(_user_avatar, other._user_avatar, sizeof(_user_avatar));
	_user_birth = other._user_birth;
	strncpy(_user_signature, other._user_signature, sizeof(_user_signature));

	return *this;
}

// 等于运算符重载函数
bool UserData::operator==(const UserData & other) const
{
	if (strcmp(_user_id, other._user_id) == 0) return true;
	else return false;
}

// 转换成XML格式
void UserData::toXML(const UserData & user, pugi::xml_node & node)
{
	// 添加节点[data]
	pugi::xml_node node_user = node.append_child("user");
	// 添加属性
	node_user.append_attribute("id") = user._user_id;

	// 添加所有子节点
	node_user.append_child("name").append_child(pugi::node_pcdata).set_value(user._user_name);
	node_user.append_child("sex").append_child(pugi::node_pcdata).set_value(userSexToStr(user._user_sex).c_str());
	node_user.append_child("avatar").append_child(pugi::node_pcdata).set_value(user._user_avatar);
	node_user.append_child("birth").append_child(pugi::node_pcdata).set_value(to_string(user._user_birth).c_str());
	node_user.append_child("signature").append_child(pugi::node_pcdata).set_value(user._user_signature);
}

// XML解析转换为UserData类型
UserData UserData::toUserData(const xml_node & node)
{
	string id = node.attribute("id").value();
	string name = node.child_value("name");
	USER_SEX sex = strToUserSex(node.child_value("sex"));
	string avatar = node.child_value("avatar");
	uint32_t birth = stoi(node.child_value("birth"));
	string signature = node.child_value("signature");

	return UserData(id.c_str(), name.c_str(), sex, avatar.c_str(), birth, signature.c_str());
}

// 列表初始化构造函数
GroupData::GroupData(const char * id) { strncpy(_group_id, id, sizeof(_group_id)); };

// 列表初始化构造函数
GroupData::GroupData(const char * id, const char * name, const char * avatar, const uint32_t& time, const char * introduction, const char * owner, const uint32_t& number) : _group_time(time), _group_number(number)
{
	strncpy(_group_id, id, sizeof(_group_id));
	strncpy(_group_name, name, sizeof(_group_name));
	strncpy(_group_avatar, avatar, sizeof(_group_avatar));
	strncpy(_group_introduction, introduction, sizeof(_group_introduction));
	strncpy(_group_owner, owner, sizeof(_group_owner));
}

// 拷贝构造函数
GroupData::GroupData(const GroupData & group) : _group_time(group._group_time), _group_number(group._group_number)
{
	strncpy(_group_id, group._group_id, sizeof(_group_id));
	strncpy(_group_name, group._group_name, sizeof(_group_name));
	strncpy(_group_avatar, group._group_avatar, sizeof(_group_avatar));
	strncpy(_group_introduction, group._group_introduction, sizeof(_group_introduction));
	strncpy(_group_owner, group._group_owner, sizeof(_group_owner));
}

// 赋值符号重载函数
GroupData & GroupData::operator=(const GroupData & group)
{
	strncpy(_group_id, group._group_id, sizeof(_group_id));
	strncpy(_group_name, group._group_name, sizeof(_group_name));
	strncpy(_group_avatar, group._group_avatar, sizeof(_group_avatar));
	_group_time = group._group_time;
	strncpy(_group_introduction, group._group_introduction, sizeof(_group_introduction));
	strncpy(_group_owner, group._group_owner, sizeof(_group_owner));
	_group_number = group._group_number;

	return *this;
}

// 等于运算符重载函数
bool GroupData::operator==(const GroupData & group) const
{
	if (strcmp(_group_id, group._group_id) == 0) return true;
	else return false;
}

// 转换成XML格式
void GroupData::toXML(const GroupData & group, pugi::xml_node & node)
{
	// 添加节点[data]
	pugi::xml_node node_group = node.append_child("group");
	// 添加属性
	node_group.append_attribute("id") = group._group_id;

	// 添加所有子节点
	node_group.append_child("name").append_child(pugi::node_pcdata).set_value(group._group_name);
	node_group.append_child("avatar").append_child(pugi::node_pcdata).set_value(group._group_avatar);
	node_group.append_child("time").append_child(pugi::node_pcdata).set_value(to_string(group._group_time).c_str());
	node_group.append_child("introduction").append_child(pugi::node_pcdata).set_value(group._group_introduction);
	node_group.append_child("owner").append_child(pugi::node_pcdata).set_value(group._group_owner);
	node_group.append_child("number").append_child(pugi::node_pcdata).set_value(to_string(group._group_number).c_str());
}

// XML解析转换为GroupData类型
GroupData GroupData::toGroupData(const xml_node & node)
{
	string id = node.attribute("id").value();
	string name = node.child_value("name");
	string avatar = node.child_value("avatar");
	uint32_t time = stoi(node.child_value("time"));
	string introduction = node.child_value("introduction");
	string owner = node.child_value("owner");
	uint32_t number = stoi(node.child_value("number"));

	return GroupData(id.c_str(), name.c_str(), avatar.c_str(), time, introduction.c_str(), owner.c_str(), number);
}

// 列表初始化构造函数
RequestData::RequestData(const uint64_t& id) : _request_id(id) {  };

// 列表初始化构造函数
RequestData::RequestData(const uint64_t& id, const ID& from, const ID& to, const char * verify, const REQUEST_STATUS& status, const uint32_t & time) : _request_id(id), _from_id(from), _to_id(to), _request_status(status), _request_time(time)
{
	strncpy(_verify_message, verify, sizeof(_verify_message));
}

// 拷贝构造函数
RequestData::RequestData(const RequestData & request) : _request_id(request._request_id), _request_status(request._request_status), _from_id(request._from_id), _to_id(request._to_id), _request_time(request._request_time)
{
	strncpy(_verify_message, request._verify_message, sizeof(_verify_message));
}

// 赋值符号重载函数
RequestData & RequestData::operator=(const RequestData & request)
{
	_request_id = request._request_id;
	_from_id = request._from_id;
	_to_id = request._to_id;
	strncpy(_verify_message, request._verify_message, sizeof(_verify_message));
	_request_status = request._request_status;
	_request_time = request._request_time;

	return *this;
}

// 等于运算符重载函数
bool RequestData::operator==(const RequestData & request) const
{
	if (_request_id == request._request_id) return true;
	else return false;
}

// 转换成XML格式
void RequestData::toXML(const RequestData & request, pugi::xml_node & node)
{
	// 添加节点[data]
	pugi::xml_node node_request = node.append_child("user");
	// 添加属性
	node_request.append_attribute("id") = request._request_id;

	// 添加所有子节点
	node_request.append_child("from").append_child(pugi::node_pcdata).set_value(request._from_id.retId());
	node_request.append_child("to").append_child(pugi::node_pcdata).set_value(request._to_id.retId());
	node_request.append_child("verify").append_child(pugi::node_pcdata).set_value(request._verify_message);
	node_request.append_child("status").append_child(pugi::node_pcdata).set_value(requestStatusToStr(request._request_status).c_str());
	node_request.append_child("time").append_child(pugi::node_pcdata).set_value(to_string(request._request_time).c_str());

	// 添加子节点属性
	node_request.child("from").append_attribute("type") = ID::idTypeToStr(request._from_id.retIdType()).c_str();
	node_request.child("to").append_attribute("type") = ID::idTypeToStr(request._to_id.retIdType()).c_str();
}

// XML解析转换为RequestData类型
RequestData RequestData::toRequestData(const xml_node & node)
{
	uint32_t id = stoi(node.attribute("id").value());
	ID from = ID(node.child_value("from"), ID::strToIdType(node.child("from").attribute("type").value()));
	ID to = ID(node.child_value("to"), ID::strToIdType(node.child("to").attribute("type").value()));
	string verify = node.child_value("verify");
	REQUEST_STATUS status = strToRequestStatus(node.child_value("status"));
	uint32_t time = stoll(node.child_value("time"));

	return RequestData(id, from, to, verify.c_str(), status, time);
}

// 列表初始化构造函数
MessageData::MessageData(const uint64_t & id) : _message_id(id) {  };

// 列表初始化构造函数
MessageData::MessageData(const uint64_t & id, const DATA_TYPE& type, const char * from, /*const char * to, */const uint64_t& time, const char * data) : _message_id(id), _message_type(type), _send_time(time)
{
	strncpy(_from_id, from, sizeof(_from_id));
	// strncpy(_to_id, to, sizeof(_to_id));
	strncpy(_message_data, data, sizeof(_message_data));
}

// 拷贝构造函数
MessageData::MessageData(const MessageData& message) : _message_id(message._message_id), _message_type(message._message_type), _send_time(message._send_time)
{
	strncpy(_from_id, message._from_id, sizeof(_from_id));
	// strncpy(_to_id, message._to_id, sizeof(_to_id));
	strncpy(_message_data, message._message_data, sizeof(_message_data));
}

// 赋值运算符重载函数
MessageData & MessageData::operator=(const MessageData& message)
{
	_message_id = message._message_id;
	_message_type = message._message_type;
	strncpy(_from_id, message._from_id, sizeof(_from_id));
	// strncpy(_to_id, message._to_id, sizeof(_to_id));
	_send_time = message._send_time;
	strncpy(_message_data, message._message_data, sizeof(_message_data));

	return *this;
}

// 等于运算符重载函数
bool MessageData::operator==(const MessageData & message) const
{
	if (_message_id == message._message_id) return true;
	else return false;
}

// 转换成XML格式
void MessageData::toXML(const MessageData& message, pugi::xml_node & node)
{
	// 添加节点[data]
	pugi::xml_node node_message = node.append_child("message");
	// 添加属性
	node_message.append_attribute("id") = message._message_id;
	node_message.append_attribute("type") = dataTypeToStr(message._message_type).c_str();

	// 添加所有子节点
	node_message.append_child("from").append_child(pugi::node_pcdata).set_value(message._from_id);
	// node_message.append_child("to").append_child(pugi::node_pcdata).set_value(message._to_id);
	node_message.append_child("data").append_child(pugi::node_pcdata).set_value(to_string(message._send_time).c_str());
	node_message.append_child("time").append_child(pugi::node_pcdata).set_value(message._message_data);

}

// XML解析转换为UserData类型
MessageData MessageData::toMessageData(const xml_node & node)
{
	uint32_t id = stoi(node.attribute("id").value());
	MessageData::DATA_TYPE type = strToDataType(node.attribute("type").value());
	string from = node.child_value("from");
	// string to = node.child_value("to");
	uint64_t time = stoll(node.child_value("time"));
	string data = node.child_value("data");

	return MessageData(id, type, from.c_str(), /*to.c_str(), */time, data.c_str());
}


// 列表初始化构造函数
DingDongData::DingDongData(const UserData& user, const UserList& friends, const GroupList& groups, const GroupMemberList& members, const RequestList& requests, const MessageList & messages) : _myself(user), _friends(friends), _groups(groups), _members(members), _requests(requests), _messages(messages)
{

}

// 拷贝构造函数
DingDongData::DingDongData(const DingDongData& other) : _myself(other._myself), _friends(other._friends), _groups(other._groups), _members(other._members), _requests(other._requests), _messages(other._messages)
{

}

// 赋值运算符重载函数
DingDongData & DingDongData::operator=(const DingDongData &other)
{
	_myself = other._myself;
	_friends = other._friends;
	_groups = other._groups;
	_members = other._members;
	_requests = other._requests;
	_messages = other._messages;

	return *this;
}

// 好友列表转换成XML格式
void DingDongData::friendListToXML(const UserList & friends, pugi::xml_node & node)
{
	// 添加子节点
	pugi::xml_node node_list = node.append_child("friendlist");

	// 循环往子节点添加内容
	for (auto p = friends.begin(); p != friends.end(); p++)
		UserData::toXML(*p, node_list);
}

// XML解析转换为UserList类型
UserList DingDongData::toFriendList(const xml_node & node)
{
	UserList friendlist;
	for (pugi::xml_node p = node.child("user"); p; p = p.next_sibling())
	{
		friendlist.push_back(UserData::toUserData(p));
	}

	return friendlist;
}

// 群列表转换成XML格式
void DingDongData::groupListToXML(const GroupList & grouplist, pugi::xml_node & node)
{
	// 添加子节点
	pugi::xml_node node_list = node.append_child("grouplist");

	// 循环往子节点添加内容
	for (auto p = grouplist.begin(); p != grouplist.end(); p++)
		GroupData::toXML(*p, node_list);
}

// XML解析转换为UserList类型
GroupList DingDongData::toGroupList(const xml_node & node)
{
	GroupList grouplist;
	for (pugi::xml_node p = node.child("group"); p; p = p.next_sibling())
	{
		grouplist.push_back(GroupData::toGroupData(p));
	}

	return grouplist;
}

// 群成员列表转换成XML格式
void DingDongData::groupMemberListToXML(const GroupMemberList & memberlist, pugi::xml_node & node)
{
	// 添加子节点
	pugi::xml_node node_list = node.append_child("groupmemberslist");

	// 循环往子节点添加内容
	for (auto p = memberlist.begin(); p != memberlist.end(); ++p)
	{
		// 添加群子节点
		pugi::xml_node node_group = node_list.append_child("group");
		node_group.append_attribute("id") = p->first.c_str();

		// 往群子节点插入数据
		for (auto i = p->second.begin(); i != p->second.end(); ++i)
			UserData::toXML(*i, node_group);
	}
}

// XML解析转换为UserList类型
GroupMemberList DingDongData::toGroupMemberList(const xml_node & node)
{
	GroupMemberList memberlist;
	for (pugi::xml_node p = node.child("group"); p; p = p.next_sibling())
	{
		memberlist.insert(make_pair(p.attribute("id").value(), DingDongData::toFriendList(p)));
	}
	return memberlist;
}

// 请求列表转换成XML格式
void DingDongData::friendRequestListToXML(const RequestList & requestlist, pugi::xml_node & node)
{
	// 添加子节点
	pugi::xml_node node_list = node.append_child("requestlist");

	for (auto p = requestlist.begin(); p != requestlist.end(); p++)
		RequestData::toXML(*p, node_list);
}

// XML解析转换为RequestList类型
RequestList DingDongData::toRequestList(const xml_node & node)
{
	RequestList requestlist;
	for (pugi::xml_node p = node.child("request"); p; p = p.next_sibling())
	{
		requestlist.push_back(RequestData::toRequestData(p));
	}

	return requestlist;
}

// 转换成XML格式 主要是区分出群账号消息记录和用户账号消息记录
void DingDongData::messageListToXML(const MessageList & messagelist, pugi::xml_node & node)
{
	// 添加子节点
	pugi::xml_node node_list = node.append_child("messagelist");

	// 往子节点添加内容
	for (auto message : messagelist)
	{
		// 添加消息队列子节点
		pugi::xml_node node_id = node_list.append_child("null");
		node_id.append_attribute("id") = message.first.retId();

		if (message.first.retIdType() == ID::user) node_id.set_name("user");
		else if (message.first.retIdType() == ID::group) node_id.set_name("group");

		// 往消息队列子节点添加内容
		for (auto p = message.second.begin(); p != message.second.end(); ++p)
			MessageData::toXML(*p, node_id);
	}
}

// XML解析转换为MessageList类型
MessageList DingDongData::toMessageList(const xml_node & node)
{
	// 消息记录
	MessageList message;

	// 获取消息
	for (pugi::xml_node p = node.first_child(); p; p = p.next_sibling())
	{
		// 消息队列
		list<MessageData> message_data;

		// 获取消息队列
		for (pugi::xml_node q = p.first_child(); q; q = q.next_sibling())
			message_data.push_back(MessageData::toMessageData(q));

		string id = p.attribute("id").value();

		// 添加id和对应的消息队列
		if (strcmp(p.name(), "user") == 0)
			message.insert(make_pair((ID(id.c_str(), ID::user)), message_data));
		else if (strcmp(p.name(), "group") == 0)
			message.insert(make_pair((ID(id.c_str(), ID::group)), message_data));
	}

	return message;
}

// 转换成XML格式
void DingDongData::toXML(const DingDongData & data, pugi::xml_node & node)
{
	// 添加子节点
	pugi::xml_node node_dd_data = node.append_child("ddData");

	UserData::toXML(data._myself, node_dd_data);
	DingDongData::friendListToXML(data._friends, node_dd_data);
	DingDongData::groupListToXML(data._groups, node_dd_data);
	DingDongData::groupMemberListToXML(data._members, node_dd_data);
	DingDongData::friendRequestListToXML(data._requests, node_dd_data);
	DingDongData::messageListToXML(data._messages, node_dd_data);
}

// XML解析转换为DingDongData类型
DingDongData DingDongData::toDingDongData(const xml_node & node)
{
	DingDongData qq_data;

	qq_data._myself = UserData::toUserData(node.child("user"));
	qq_data._friends = DingDongData::toFriendList(node.child("friendlist"));
	qq_data._groups = DingDongData::toGroupList(node.child("grouplist"));
	qq_data._members = DingDongData::toGroupMemberList(node.child("groupmemberslist"));
	qq_data._requests = DingDongData::toRequestList(node.child("requestlist"));
	qq_data._messages = DingDongData::toMessageList(node.child("messagelist"));

	return qq_data;
}

// 更新图片路径
void DingDongData::updateImage(void)
{
	// 获取用户头像
	_images.push_back(_myself.retAvatar());

	// 获取好友头像
	for (auto &p : _friends)
		_images.push_back(p.retAvatar());

	// 获取群头像
	for (auto &p : _groups)
		_images.push_back(p.retAvatar());

	// 获取群成员头像
	for (auto &p : _members)
	{
		for (auto &q : p.second)
			_images.push_back(q.retAvatar());
	}

	// 获取群头像
	for (auto &p : _messages)
	{
		for (auto &q : p.second)
		{
			if (q.retType() == MessageData::DATA_TYPE::image)
				_images.push_back(q.retData());
		}
	}

	// 去重
	std::sort(_images.begin(), _images.end());
	auto end_unique = std::unique(_images.begin(), _images.end());
	_images.erase(end_unique, _images.end());

}