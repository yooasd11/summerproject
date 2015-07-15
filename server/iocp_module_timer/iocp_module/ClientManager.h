#pragma once
class ClientManager
{
private:
	std::map<int, int> mappingClient;
	Lock* key;
	int clntCnt;
public:
	ClientData clientdata[MaxClient];
	ClientManager();
	~ClientManager();
	void mapping(int);
	int retIndex(int);
	void removesocket(int);
	
};

