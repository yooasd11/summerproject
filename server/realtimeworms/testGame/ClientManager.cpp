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
	//Ŭ���̾�Ʈ�� ���ӵǾ����� �⺻���� ���������� �Էµȴ�...
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
	printf("%d ���������� �����˴ϴ�\n", clnt);
	this->mappingClient.erase(clnt);
	this->key->UNLOCK();
	return;
}
