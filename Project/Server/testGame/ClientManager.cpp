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
	LOCKING(IocpConstructor::ObjectKey);
	this->indexing[clnt] = IocpConstructor::ObjectCount;
	this->mappingClient[IocpConstructor::ObjectCount] = std::shared_ptr<USER>(new USER(IocpConstructor::ObjectCount,clnt,  100, rand() % 640 + 1.0f, rand() % 320 + 1.0f));
	IocpConstructor::ObjectCount++;
	return;
}

std::shared_ptr<USER> ClientManager::retUser(int uid)
{
	return this->mappingClient[uid];
}

int ClientManager::retUser(SOCKET clnt)
{
	return this->indexing[clnt];
}

void ClientManager::removesocket(int uid)
{
	LOCKING(this->key);
	this->mappingClient.erase(uid);
	printf("유저나감12312312\n");
	return;
}

