#pragma once
class ClientManager
{
private:
	Lock* key;
public:
	std::map<SOCKET, USER> mappingClient;
	ClientManager();
	~ClientManager();
	void mapping(SOCKET);
	USER retUser(SOCKET);
	void removesocket(SOCKET);
};

