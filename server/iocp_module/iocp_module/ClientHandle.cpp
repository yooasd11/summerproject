#include "stdafx.h"
#include "ClientHandle.h"


ClientHandle::ClientHandle()
{
	this->flags = 0;
	this->addrLen = sizeof(this->clntAdr);
}


ClientHandle::~ClientHandle()
{
}


void ClientHandle::Copy(){

	this->handleinfo = new HandleData;
	this->handleinfo->ClntSock = this->ClntSock;
	memcpy(&(this->handleinfo->clntAdr), &(this->clntAdr), this->addrLen);
	return;
}

void ClientHandle::ReadMode(){
	this->ioInfo = new IoData;
	memset(&(this->ioInfo->overlapped), 0, sizeof(OVERLAPPED));
	this->ioInfo->wsaBuf.len = BufSize;
	this->ioInfo->wsaBuf.buf = this->ioInfo->buf;
	this->ioInfo->rwMode = READ;
	return;
}

void ClientHandle::WriteMode(){
	memset(&(this->ioInfo->overlapped), 0, sizeof(OVERLAPPED));
	this->ioInfo->wsaBuf.len = this->bytesTrans;
	this->ioInfo->rwMode = WRITE;
	return;
}
