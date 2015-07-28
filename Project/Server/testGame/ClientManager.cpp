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

//void ClientManager::UserSetting(int _uid, int _current, int _total, char* _msg, std::shared_ptr<USER> user)
//{
//	LOCKING(this->key);
//	user.get()->setTotal(_total);
//	user.get()->setCurrent(_current);
//	user.get()->setBuffer(_msg, _total);
//	user.get()->uid = _uid;
//	return;
//}
//
//char* ClientManager::getUserBuffer(std::shared_ptr<USER> user)
//{
//	return user.get()->getBuffer();
//}
//
//int ClientManager::getUserTotal(std::shared_ptr<USER> user)
//{
//	return user.get()->getCurrent();
//}
//
//int ClientManager::getUserCurrent(std::shared_ptr<USER> user)
//{
//	return user.get()->getCurrent();
//}
//
//bool ClientManager::UserConnecting(std::shared_ptr<USER> user)
//{
//	return user.get()->isConnecting();
//}
//
//void ClientManager::UserClear(std::shared_ptr<USER> user)
//{
//	user.get()->clear();
//}
//
//void ClientManager::UserPacketHandle(std::shared_ptr<USER> user)
//{
//	LOCKING(this->key);
//	user.get()->UserpacketHandle();
//	return;
//}
//
