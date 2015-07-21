#include "stdafx.h"
#include "Lock.h"


Lock::Lock()
{
	InitializeCriticalSection(&(this->cs));
}

Lock::~Lock()
{
	DeleteCriticalSection(&(this->cs));
}

void Lock::LOCK()
{
	EnterCriticalSection(&(this->cs));
}

void Lock::UNLOCK()
{
	LeaveCriticalSection(&(this->cs));
}