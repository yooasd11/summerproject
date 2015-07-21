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
	this->key->LOCK();
	//Ŭ���̾�Ʈ�� ���ӵǾ����� �⺻���� ���������� �Էµȴ�...
	USER temp(int(clnt), 100, 200.0f, 200.0f);
	this->mappingClient[clnt] = temp;
	this->clntCount += 1;
	this->key->UNLOCK();

}

USER ClientManager::retUser(SOCKET clnt)
{
	return this->mappingClient[clnt];
}

void ClientManager::removesocket(SOCKET clnt)
{
	this->key->LOCK();
	printf("%d ���������� �����˴ϴ�\n", clnt);
	this->mappingClient.erase(clnt);
	this->clntCount -= 1;
	this->key->UNLOCK();
	return;
}
