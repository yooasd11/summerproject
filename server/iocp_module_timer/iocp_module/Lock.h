#pragma once


//class Lock{
//private:
//	SRWLOCK *KEY;
//public:
//	Lock();
//	void readlock();
//	void writelock();
//	void readrelease();
//	void writerelease();
//};


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
