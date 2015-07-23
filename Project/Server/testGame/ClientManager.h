#pragma once
class ClientManager
{
private:
	Lock* key;
public:
	int clntCount;

	//std::map<SOCKET, USER> mappingClient;
	std::map<SOCKET, std::shared_ptr<USER>> mappingClient;

	ClientManager();
	~ClientManager();
	void mapping(SOCKET);

	//USER* retUser(SOCKET);
	std::shared_ptr<USER> retUser(SOCKET);

	void removesocket(SOCKET);

	//유저동작에 관한 함수..
	void UserSetting(int, int, int, char*, std::shared_ptr<USER>);
	void setUserTotal(int, std::shared_ptr<USER>);
	void setUserCurrent(int, std::shared_ptr<USER>);
	void setUserBuffer(char*, int, std::shared_ptr<USER>);
	char* getUserBuffer(std::shared_ptr<USER>);
	int getUserTotal(std::shared_ptr<USER>);
	int getUserCurrent(std::shared_ptr<USER>);
	void UserClear(std::shared_ptr<USER>);
	void UserPacketHandle(std::shared_ptr<USER>);
	bool UserConnecting(std::shared_ptr<USER>);
};



//void setTotal(int);
//void setCurrent(int);
//void setBuffer(char*, int);
//char* getBuffer();
//int getTotal();
//int getCurrent();
//bool finished();
//void clear();
//void UserpacketHandle();
//bool isConnecting();
//
////잡큐에 관한 작업정의...
//void UserMove();



