#include "stdafx.h"
#include "LockHelper.h"


LockHelper::LockHelper()
{
	this->key = new Lock();
	//이 구역을 임계구역으로 만들어줌..
	key->LOCK();
}


LockHelper::~LockHelper()
{
	this->key->UNLOCK();
	delete key;
}
