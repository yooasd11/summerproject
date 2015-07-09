#include "stdafx.h"
#include "user.h"


user::user()
{
	memset(this->storeMessage, 0, sizeof(storeMessage));
	this->totalLength = 0;
	this->Currentlength = 0;
	this->KEY = new LOCKING;
}
user::~user()
{
	delete KEY;
}

void user::addLength(int len){
	this->Currentlength += len;
	return;
}

void user::init(){
	this->Currentlength = 0;
	this->totalLength = 0;
	return;
}

void user::setCurrent(int len){
	this->Currentlength = len;
	return;
}

void user::setTotal(int len){
	this->totalLength = len;
	return;
}

void user::setLock(){
	this->KEY->LOCK();
	return;
}

void user::releaseLock(){
	this->KEY->UNLOCK();
	return;
}

int user::getCurrent(){
	return this->Currentlength;
}

int user::getTotal(){
	return this->totalLength;
}

char* user::getMessage(){
	return this->storeMessage + this->Currentlength;
}