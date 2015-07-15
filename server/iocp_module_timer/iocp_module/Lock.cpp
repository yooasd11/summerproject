#include "stdafx.h"
#include "Lock.h"



//Lock::Lock(){
//	KEY = new SRWLOCK();
//	InitializeSRWLock(this->KEY);
//}
//void Lock::readlock(){
//	AcquireSRWLockShared(this->KEY);
//}
//void Lock::writelock(){
//	AcquireSRWLockExclusive(this->KEY);
//}
//void Lock::readrelease(){
//	ReleaseSRWLockShared(this->KEY);
//}
//void Lock::writerelease(){
//	ReleaseSRWLockExclusive(this->KEY);
//}


Lock::Lock(){
	InitializeCriticalSection(&(this->cs));
}

Lock::~Lock(){
	DeleteCriticalSection(&(this->cs));
}

void Lock::LOCK(){
	EnterCriticalSection(&(this->cs));
}

void Lock::UNLOCK(){
	LeaveCriticalSection(&(this->cs));
}