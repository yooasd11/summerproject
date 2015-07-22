#include "stdafx.h"
#include "ClientManager.h"


ClientManager::ClientManager()
{
	this->clntCount = 0;
	this->key = new Lock();
}


ClientManager::~ClientManager()
{
	delete this->key;
}

void ClientManager::mapping(SOCKET clnt)
{
	LOCKING(this->key);
	
	//클라이언트가 접속되었을때 기본적인 유저정보가 입력된다...
	USER temp(int(clnt), 100, rand() % 640 + 1.0f, rand() % 320 + 1.0f);
	this->mappingClient[clnt] = temp;
	this->clntCount += 1;


}

USER ClientManager::retUser(SOCKET clnt)
{
	return this->mappingClient[clnt];
}

void ClientManager::removesocket(SOCKET clnt)
{
	LOCKING(this->key);
	printf("%d 유저정보가 삭제됩니다\n", clnt);
	this->mappingClient.erase(clnt);
	this->clntCount -= 1;
	return;
}
