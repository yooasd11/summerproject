#include "stdafx.h"
#include "ClientData.h"


ClientData::ClientData()
{
	this->Key = new Lock();
}

ClientData::~ClientData()
{
	delete this->Key;
}

void ClientData::setLock(){
	this->Key->LOCK();
	return;
}

void ClientData::releaseLock(){
	this->Key->UNLOCK();
	return;
}

void ClientData::addLength(int len){
	this->buffer.CurrentLength += len;
	return;
}

void ClientData::init(){
	this->buffer.CurrentLength = 0;
	this->buffer.TotalLength = 0;
	memset(this->buffer.buf, 0, sizeof(this->buffer.buf));
	return;
}

void ClientData::setCurrent(int len){
	this->buffer.CurrentLength = len;
	return;
}

void ClientData::setTotal(int len){
	this->buffer.TotalLength = len;
	return;
}

int ClientData::getCurrent(){
	return this->buffer.CurrentLength;
}

int ClientData::getTotal(){
	return this->buffer.TotalLength;
}

char* ClientData::getMessage(){
	return this->buffer.buf + this->buffer.CurrentLength;
}

void ClientData::printMessage(){
	printf("message : %s \n", this->buffer.buf);
	return;
}