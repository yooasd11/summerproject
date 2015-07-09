#include "stdafx.h"
#include "ClientManager.h"


ClientManager::ClientManager()
{
	this->key = new Lock;
	this->clntCnt = 1;
}


ClientManager::~ClientManager()
{
	delete this->key;
}


void ClientManager::mapping(int sock){
	this->key->LOCK();
	this->mappingClient[sock] = clntCnt++;
	this->key->UNLOCK();
	return;
}

int ClientManager::retIndex(int sock){
	//���� ���� �ɾ�����ϳ�...
	return this->mappingClient[sock];
}