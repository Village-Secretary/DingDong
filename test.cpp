// This is a test
#define __DEBUG_USER
#define __DEBUG_GROUP
#define __DEBUG_REQUEST
#define __DEBUG_MESSAGE
#define __DEBUG_DATA
#define __DEBUG_DATA_FUNCTION
#define __DEBUG_DATA_IMAGE

#ifdef __DEBUG_DATA_FUNCTION

#define __DINGDONG_DATA_DEBUG

#endif

#include "DingDongData.hpp"
#include <iostream>

int main(void)
{

	// 测试UserData类的初始化、XML转UserData、UserData转XML
#ifdef __DEBUG_USER

	pugi::xml_document doc_user;
	pugi::xml_parse_result result_user = doc_user.load_file("C:\\Users\\ZombieProcess\\Desktop\\DingDong\\xml\\user.xml");
	if (!result_user)
		return -1;
	pugi::xml_node node_user = doc_user.child("user");
	UserData user = UserData::toUserData(node_user);
	std::cout << UserData::toXMLString(user) << std::endl;

#endif

	// 测试GroupData类的初始化、XML转GroupData、GroupData转XML
#ifdef __DEBUG_GROUP

	pugi::xml_document doc_group;
	pugi::xml_parse_result result_group = doc_group.load_file("C:\\Users\\ZombieProcess\\Desktop\\DingDong\\xml\\group.xml");
	if (!result_group)
		return -1;
	pugi::xml_node node_group = doc_group.child("group");
	GroupData group = GroupData::toGroupData(node_group);
	std::cout << GroupData::toXMLString(group) << std::endl;

#endif

	// 测试RequestData类的初始化、XML转RequestData、RequestData转XML
#ifdef __DEBUG_REQUEST

	pugi::xml_document doc_request;
	pugi::xml_parse_result result_request = doc_request.load_file("C:\\Users\\ZombieProcess\\Desktop\\DingDong\\xml\\request.xml");
	if (!result_request)
		return -1;
	pugi::xml_node node_request = doc_request.child("request");
	RequestData request = RequestData::toRequestData(node_request);
	std::cout << RequestData::toXMLString(request) << std::endl;

#endif

	// 测试MessageData类的初始化、XML转MessageData、MessageData转XML
#ifdef __DEBUG_MESSAGE

	pugi::xml_document doc_message;
	pugi::xml_parse_result result_message = doc_message.load_file("C:\\Users\\ZombieProcess\\Desktop\\DingDong\\xml\\message.xml");
	if (!result_message)
		return -1;
	pugi::xml_node node_message = doc_message.child("message");
	MessageData message = MessageData::toMessageData(node_message);
	std::cout << MessageData::toXMLString(message) << std::endl;

#endif

	// 测试DingDongData类的初始化、XML转DingDongData、DingDongData转XML
#ifdef __DEBUG_DATA

	pugi::xml_document doc_data;
	pugi::xml_parse_result result_data = doc_data.load_file("C:\\Users\\ZombieProcess\\Desktop\\DingDong\\xml\\data.xml");
	if (!result_data)
		return -1;
	pugi::xml_node node_data = doc_data.child("qq");
	DingDongData data = DingDongData::toDingDongData(node_data);
	std::cout << DingDongData::toXMLString(data) << std::endl;

#endif

	// 测试DingDongData的数据增删查改功能
#ifdef __DEBUG_DATA_FUNCTION

#include <map>

	pugi::xml_document doc_data_function;
	pugi::xml_parse_result result_data_function = doc_data_function.load_file("C:\\Users\\ZombieProcess\\Desktop\\DingDong\\xml\\data.xml");
	if (!result_data_function)
		return -1;
	pugi::xml_node node_data_function = doc_data_function.child("qq");
	DingDongData data_function = DingDongData::toDingDongData(node_data_function);
	data_function.show();

	// 修改个人信息
	UserData * myself = data_function.retMyself();
	// myself->setId("123455")                              无法修改账号ID
	myself->setName("AAAA");                                // 修改昵称
	myself->setAvatar("./image/user_avatar/333.jpg");       // 修改头像
	myself->setBirth(20210713);                             // 修改生日
	myself->setSex(UserData::USER_SEX::null);               // 修改性别
	myself->setSignature("");                               // 修改个性签名

	// 添加好友
	UserData friend_1("123456789", "XiaoMing", UserData::USER_SEX::man, "./image/user_avatar/4314.jpg", 20010213, "Hello~");
	data_function.addFriend(friend_1);
	data_function.show();
	// 删除好友
	data_function.deleteFriend(friend_1);
	data_function.deleteFriend(UserData("1094311033"));
	data_function.show();

	// 添加群
	GroupData group_1("123456789", "HappyFamily", "./image/group_avatar/32.jpg", 20210812, "XXXXXXXXXX", "2248585019", 2);
	GroupData group_2("987654321", "HappyFamily", "./image/group_avatar/32.jpg", 20210812, "XXXXXXXXXX", "2248585019", 2);
	data_function.addGroup(group_1);
	data_function.addGroup(group_2);
	data_function.show();
	// 删除群
	data_function.deleteGroup(group_1);
	data_function.deleteGroup(GroupData("987654321"));
	data_function.show();

	// 添加群成员
	std::pair<std::string, UserList>member_1(std::make_pair(std::string("123456789"), std::list<UserData>{ UserData("123"), UserData("456") }));
	data_function.addGroupMember(member_1);
	data_function.addGroupMember("123456789", "789");
	data_function.show();
	// 删除群成员
	data_function.deleteGroupMember("123456789", "123");
	data_function.show();
	data_function.deleteGroupMember("123456789");
	data_function.show();

	// 添加请求
	RequestData request_1(43252, ID("123456789", ID::group), ID("987654321", ID::user), "", RequestData::REQUEST_STATUS::agree, 20210612);
	data_function.addRequest(request_1);
	// 删除请求
	data_function.show();
	data_function.deleteRequest(request_1);
	data_function.show();
	data_function.deleteRequest(RequestData(1342));
	data_function.show();

	// 添加消息
	std::pair<ID, std::list<MessageData>> mesage_1(std::make_pair(ID("123456789", ID::ID_TYPE::group), std::list<MessageData>{ MessageData(42122, MessageData::DATA_TYPE::text, "123", "456", 20010244, "Ni Hao a") }));
	std::pair<ID, std::list<MessageData>> mesage_2(std::make_pair(ID("123456789", ID::ID_TYPE::user), std::list<MessageData>{ MessageData(23242, MessageData::DATA_TYPE::text, "123", "456", 20010245, "XXX") }));
	data_function.addMessage(mesage_1);
	data_function.addMessage(mesage_2);
	data_function.addMessage(ID("216559214", ID::ID_TYPE::group), MessageData(1293, MessageData::DATA_TYPE::text, "2248585019", "216559214", 20010713161243, "is so boring too!"));
	data_function.show();
	data_function.deleteMessage(ID("123456789", ID::ID_TYPE::group), MessageData(42122));
	data_function.show();
	data_function.deleteMessage(ID("123456789", ID::ID_TYPE::user), MessageData(23242));
	data_function.show();
	data_function.deleteMessage(ID("123456789", ID::ID_TYPE::user));
	data_function.deleteMessage(ID("216559214", ID::ID_TYPE::group));
	data_function.show();

	std::cout << DingDongData::toXMLString(data_function) << std::endl;

#endif

#ifdef __DEBUG_DATA_IMAGE

	pugi::xml_document doc_image;
	pugi::xml_parse_result result_image = doc_image.load_file("C:\\Users\\ZombieProcess\\Desktop\\DingDong\\xml\\data.xml");
	if (!result_image)
		return -1;
	pugi::xml_node node_image = doc_image.child("qq");
	DingDongData data_image = DingDongData::toDingDongData(node_image);
	data_image.updateImage();

#endif

	return 0;
}