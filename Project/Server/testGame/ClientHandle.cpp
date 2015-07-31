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

void ClientHandle::Copy()
{
	this->handleinfo = new HandleData;
	this->handleinfo->ClntSock = this->ClntSock;
	memcpy(&(this->handleinfo->clntAdr), &(this->clntAdr), this->addrLen);
	return;
}

void ClientHandle::ReadMode()
{
	this->ioinfo = new IoData;
	memset(&(this->ioinfo->overlapped), 0, sizeof(OVERLAPPED));
	this->ioinfo->wsaBuf.len = BUFSIZE;
	this->ioinfo->wsaBuf.buf = this->ioinfo->buf;
	this->ioinfo->rwMode = READ;
	return;
}

void ClientHandle::WriteMode()
{
	memset(&(this->ioinfo->overlapped), 0, sizeof(OVERLAPPED));
	this->ioinfo->wsaBuf.len = this->bytesTrans;
	this->ioinfo->rwMode = WRITE;
	return;
}