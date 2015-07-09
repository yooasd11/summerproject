#pragma once
class Lock
{
private:
	CRITICAL_SECTION cs;
public:
	Lock();
	~Lock();
	void LOCK();
	void UNLOCK();
};

