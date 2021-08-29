// This is a test
// #define __DEBUG_USER
// #define __DEBUG_GROUP
// #define __DEBUG_REQUEST
// #define __DEBUG_MESSAGE
// #define __DEBUG_DATA
// #define __DEBUG_DATA_FUNCTION
// #define __DEBUG_DATA_IMAGE
 //#define __DEBUG_TRANSFERDATA_CREATE_AND_PARSE
 #define __DEBUG_TRANSFERDATA_SEND


// 测试DingDongData的数据增删查改功能，这里为打开DingDongData类的Show函数
#ifdef __DEBUG_DATA_FUNCTION

#define __DINGDONG_DATA_DEBUG

#endif

// 测速客户端和服务器之间的转发协议
#ifdef __DEBUG_TRANSFERDATA_SEND

#include <winsock2.h>
#include <fstream>
#include <cstdlib>

#pragma comment(lib,"ws2_32.lib")

void initialization();              // 初始化Winsock2.2版本

constexpr uint32_t BUFF_MAX = 1024;

#define __DEBUG_TRANSFERDATA_SEND_SERVER			// 测试服务器
//#define __DEBUG_TRANSFERDATA_SEND_CLIENT			// 测试客户端

#ifdef __DEBUG_TRANSFERDATA_SEND_SERVER

#define __DD_SERVER

#endif

#ifdef __DEBUG_TRANSFERDATA_SEND_CLIENT

#define __DD_CLIENT

#endif


#endif

#include "DingDongData.hpp"
#include "TransferData.hpp"
#include <iostream>

uint32_t main(void)
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

	// 测试DingDongData的图片管理功能
#ifdef __DEBUG_DATA_IMAGE

	pugi::xml_document doc_image;
	pugi::xml_parse_result result_image = doc_image.load_file("C:\\Users\\ZombieProcess\\Desktop\\DingDong\\xml\\data.xml");
	if (!result_image)
		return -1;
	pugi::xml_node node_image = doc_image.child("qq");
	DingDongData data_image = DingDongData::toDingDongData(node_image);
	data_image.updateImage();

#endif

#ifdef __DEBUG_TRANSFERDATA_CREATE_AND_PARSE

	std::string request = requestMessage(TRANSFER_TYPE::_send, TRANSFER_STATUS::request, ID("12345", ID::user), ID("54321", ID::group), "");
	
	uint32_t type;
	auto p = parseTypeHeader(type, request.c_str());
	if (type != 1)
		std::cout << "Error" << std::endl;

	std::string from, to, path;
	uint64_t m_len;
	p = parseMessageHeader(from, to, m_len, p);
	std::cout << "from: " << from << " to: " << to << " len: " << m_len << std::endl;

	request = requestImage("54321", "12345", "./image/user_avatar/333.jpg");
	p = parseTypeHeader(type, request.c_str());
	if (type != 1)
		std::cout << "Error" << std::endl;
	
	p = parseImageHeader(from, to, path, m_len, p);
	std::cout << "from: " << from << " to: " << to << " path: " << path << " len: " << m_len << std::endl;

#endif

	// 测速客户端和服务器之间的转发协议
#ifdef __DEBUG_TRANSFERDATA_SEND

	using std::cout;
	using std::endl;
	using std::cin;
	using std::flush;

	// 初始化Winsock2.2版本
	initialization();

	//定义服务端套接字，接受请求套接字	
	SOCKET s_server;

	//服务端地址客户端地址	
	sockaddr_in server_addr;

	//填充服务端信息	
	server_addr.sin_family = AF_INET;
#ifdef __DEBUG_TRANSFERDATA_SEND_SERVER

	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

#endif
#ifdef __DEBUG_TRANSFERDATA_SEND_CLIENT

	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

#endif
	server_addr.sin_port = htons(9090);

	//创建套接字	
	s_server = socket(AF_INET, SOCK_STREAM, 0);

#ifdef __DEBUG_TRANSFERDATA_SEND_SERVER

	// 连接套接字
	SOCKET s_accept;

	// 连接客户端地址信息
	sockaddr_in accept_addr;

	// 绑定本机地址和端口等信息给套接字
	if (bind(s_server, (sockaddr *)&server_addr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		cout << "bind failed!" << endl;
		WSACleanup();
	}
	else
		cout << "bind succeeded!" << endl;

	//设置套接字为监听状态	
	if (listen(s_server, SOMAXCONN) < 0)
	{
		cout << "listen failed!" << endl;
		WSACleanup();
	}
	else
		cout << "listen succeeded!" << endl;

	cout << "The server is listening for connections, please wait..." << endl;	//接受连接请求	

	int len = sizeof(sockaddr);

	// 当系统空闲时，将接受客户机的连接请求，获得连接信息，创建新的套接字，并返回该套接字的文件描述符
	s_accept = accept(s_server, (sockaddr *)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR)
	{
		cout << "connect failed" << endl;
		WSACleanup();
		return 0;
	}

	cout << "connection established, ready to receive data..." << endl;	
	
	char recv_buff[BUFF_MAX]{ 0 };				// 缓存区：recv函数接受数据的缓存区

	recvDDServer(s_accept, recv_buff, BUFF_MAX);

	cout << "end of receiving data." << endl;

	closesocket(s_accept);

#endif

#ifdef __DEBUG_TRANSFERDATA_SEND_CLIENT

	// 根据server_addr所提供的信息，向一个服务器发送连接请求
	if (connect(s_server, (SOCKADDR *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "connect to server failed" << endl;
		WSACleanup();
	}
	else
	{
		cout << "connect to server succeeded" << endl;
	}

	std::string temp_path = "C:\\Users\\ZombieProcess\\Desktop\\DingDong\\image\\client\\";
	uint32_t count = 0;
	while (true)
	{
		uint32_t num = 0, type = 0;

		cout << "enter a number to send type: " << flush;
		cin >> type;

		cout << "enter a number to send number: " << flush;
		cin >> num;

		if (type == 2 && num > 15) num = 45;

		for (uint32_t i = 0; i < num; i++)
		{
			int send_len;
			if (type == 1)
			{

				DDHeader m_header(DDHeader::DDHEADER_TYPE::message, ID(std::to_string(i).c_str(), ID::user), ID(std::to_string(i + 1).c_str(), ID::group));
				auto data = m_header.createSendData(0, retNowTime(), MessageData::text, "fioafiowafjawjfaofjopaw");
				std::string message = m_header.createHeader(m_header.createTextData(TRANSFER_TYPE::_send, TRANSFER_STATUS::_request, data));

				send_len = send(s_server, message.c_str(), message.size(), 0);
				if (send_len != message.size())
					cout << "error: inconsistent data" << send_len << " data len: " << message.size() << endl;
			}
			if (type == 2)
			{
				std::string path, imagefile;
				DDHeader m_header(DDHeader::DDHEADER_TYPE::message, ID(std::to_string(i).c_str(), ID::user), ID(std::to_string(i + 1).c_str(), ID::group));
				auto data = m_header.createSendData(0, retNowTime(), MessageData::image, (temp_path + std::to_string(i) + ".jpg").c_str(), 0, path, imagefile);
				std::string message = m_header.createHeader(m_header.createTextData(TRANSFER_TYPE::_send, TRANSFER_STATUS::_request, data));

				send_len = send(s_server, message.c_str(), message.size(), 0);
				if (send_len != message.size())
					cout << "error: inconsistent data" << send_len << " data len: " << message.size() << endl;

				DDHeader i_header(DDHeader::DDHEADER_TYPE::image, ID(std::to_string(i).c_str(), ID::user), ID(std::to_string(i + 1).c_str(), ID::group));
				message = i_header.createHeader(i_header.createImageData(path.c_str(), imagefile.c_str(), 0));
				send_len = send(s_server, message.c_str(), message.size(), 0);
				if (send_len != message.size())
					cout << "error: inconsistent data  send len: " << send_len << " data len: " << message.size() << endl;
			}

			if (send_len < 0)
				cout << "send failed! error code: " << WSAGetLastError() << endl;
			else
				cout << "[" << count << "]" << " send succeeded" << "\tsend len: " << send_len << endl;
			count += 1;
			// 根据发送的文件大小来觉得等待的时间
			uint32_t time_num = 0;

			//Sleep(150);
		}
	}



#endif

	//关闭套接字	
	closesocket(s_server);

	//释放dll资源	
	WSACleanup();

	system("pause");
#endif

	return 0;
}

// 测速客户端和服务器之间的转发协议，这里显示定义的初始化WinSock函数
#ifdef __DEBUG_TRANSFERDATA_SEND

void initialization() {

	using std::cout;
	using std::endl;

	//初始化套接字库	
	WORD w_req = MAKEWORD(2, 2);//版本号	
	WSADATA wsadata;
	uint32_t err;

	err = WSAStartup(w_req, &wsadata);
	if (err != 0)
		cout << "initialization socket failed" << endl;
	else
		cout << "initialization socket succeeded" << endl;

	 //检测版本号	
	 if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	 {
	 	cout << "socket version failed！" << endl;
	 	WSACleanup();
	 }
	 else
	 	cout << "Socket version succeeded！" << endl;
}

#endif