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
};



