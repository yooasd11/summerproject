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
	
	//Ŭ���̾�Ʈ�� ���ӵǾ����� �⺻���� ���������� �Էµȴ�...
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
	printf("%d ���������� �����˴ϴ�\n", clnt);
	this->mappingClient.erase(clnt);
	this->clntCount -= 1;
	return;
}
