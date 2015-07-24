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
	//스마트 포인터의 생성...
	this->mappingClient[clnt] = std::shared_ptr<USER>(new USER(int(clnt), 100, rand() % 640 + 1.0f, rand() % 320 + 1.0f));
	this->clntCount += 1;
}

std::shared_ptr<USER> ClientManager::retUser(SOCKET clnt)
{
	return this->mappingClient[clnt];
}


void ClientManager::removesocket(SOCKET clnt)
{
	LOCKING(this->key);
	this->mappingClient.erase(clnt);
	this->clntCount -= 1;
	return;
}

void ClientManager::UserSetting(int _uid, int _current, int _total, char* _msg, std::shared_ptr<USER> user)
{
	LOCKING(this->key);
	user.get()->setTotal(_total);
	user.get()->setCurrent(_current);
	user.get()->setBuffer(_msg, _total);
	user.get()->uid = _uid;
	return;
}

char* ClientManager::getUserBuffer(std::shared_ptr<USER> user)
{
	return user.get()->getBuffer();
}

int ClientManager::getUserTotal(std::shared_ptr<USER> user)
{
	return user.get()->getCurrent();
}

int ClientManager::getUserCurrent(std::shared_ptr<USER> user)
{
	return user.get()->getCurrent();
}

bool ClientManager::UserConnecting(std::shared_ptr<USER> user)
{
	return user.get()->isConnecting();
}

void ClientManager::UserClear(std::shared_ptr<USER> user)
{
	user.get()->clear();
}

void ClientManager::UserPacketHandle(std::shared_ptr<USER> user)
{
	LOCKING(this->key);
	user.get()->UserpacketHandle();
	return;
}

