#include "stdafx.h"
#include "HandleClient.h"


HandleClient::HandleClient()
{
	this->addrLen = sizeof(this->clntAdr);
}


HandleClient::~HandleClient()
{

}

void HandleClient::setClntSock(SOCKET client){
	this->handleInfo->hClntSock = client;

}

void HandleClient::copy(){
	handleInfo = new PER_HANDLE_DATA;
	handleInfo->hClntSock = this->hClntSock;
	memcpy(&(this->handleInfo->clntAdr), &(this->clntAdr), this->addrLen);
	return;
}

void HandleClient::ReadMode(){
	this->ioInfo = new PER_IO_DATA;
	memset(&(this->ioInfo->overlapped), 0, sizeof(OVERLAPPED));
	this->ioInfo->wsaBuf.len = BUF_SIZE;
	this->ioInfo->wsaBuf.buf = this->ioInfo->buf;
	this->ioInfo->rwMode = READ;
	return;
}