#include "stdafx.h"
#include "LockHelper.h"


LockHelper::LockHelper()
{
	this->key = new Lock();
	//�� ������ �Ӱ豸������ �������..
	key->LOCK();
}


LockHelper::~LockHelper()
{
	this->key->UNLOCK();
	delete key;
}
