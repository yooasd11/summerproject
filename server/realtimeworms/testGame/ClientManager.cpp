#include "stdafx.h"
#include "ClientManager.h"


ClientManager::ClientManager()
{
	this->key = new Lock();
}


ClientManager::~ClientManager()
{
	delete this->key;
}

void ClientManager::mapping(SOCKET clnt)
{
	this->key->LOCK();
	//클라이언트가 접속되었을때 기본적인 유저정보가 입력된다...
	USER temp(int(clnt), 100, 200, 200);
	this->mappingClient[clnt] = temp;
	this->key->UNLOCK();

}

USER ClientManager::retUser(SOCKET clnt)
{
	return this->mappingClient[clnt];
}

void ClientManager::removesocket(SOCKET clnt)
{
	this->key->LOCK();
	printf("%d 유저정보가 삭제됩니다\n", clnt);
	this->mappingClient.erase(clnt);
	this->key->UNLOCK();
	return;
}
