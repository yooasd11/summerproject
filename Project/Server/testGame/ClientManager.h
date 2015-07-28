#pragma once
class ClientManager
{
private:
	Lock* key;
public:
	int clntCount;

	//std::map<SOCKET, USER> mappingClient;
	std::map<int, std::shared_ptr<USER>> mappingClient;
	std::map<SOCKET, int> indexing;
	ClientManager();
	~ClientManager();

	void mapping(SOCKET);
	std::shared_ptr<USER> retUser(int);
	int retUser(SOCKET);
	void removesocket(int);

	//유저동작에 관한 함수..
	//void UserSetting(int, int, int, char*, std::shared_ptr<USER>);
	//void setUserTotal(int, std::shared_ptr<USER>);
	//void setUserCurrent(int, std::shared_ptr<USER>);
	//void setUserBuffer(char*, int, std::shared_ptr<USER>);
	//char* getUserBuffer(std::shared_ptr<USER>);
	//int getUserTotal(std::shared_ptr<USER>);
	//int getUserCurrent(std::shared_ptr<USER>);
	//void UserClear(std::shared_ptr<USER>);
	//void UserPacketHandle(std::shared_ptr<USER>);
	//bool UserConnecting(std::shared_ptr<USER>);
};



