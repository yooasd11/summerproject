#include "stdafx.h"
#include "ManageClient.h"


ManageClient::ManageClient()
{
	this->ClntCnt = 0;
	this->KEY = new LOCKING;
}
ManageClient::~ManageClient()
{
	delete this->KEY;
}
void ManageClient::mapping(int sock){
	
	this->KEY->LOCK();
	this->mappingClient[sock] = this->ClntCnt;
	this->ClntCnt += 1;
	this->KEY->UNLOCK();
	return;
}

int ManageClient::retIndex(int sock){
	return this->mappingClient[sock];
}


