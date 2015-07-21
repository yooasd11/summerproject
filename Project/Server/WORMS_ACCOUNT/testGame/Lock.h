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

class LockHelper{
private:
	Lock* key;
public:
	LockHelper(Lock* lc){
		key = lc;
		key->LOCK();
	}
	~LockHelper(){
		key->UNLOCK();
	}
};