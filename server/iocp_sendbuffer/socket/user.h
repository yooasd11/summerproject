#pragma once

class user
{
private:
	char storeMessage[BUF_SIZE];
	int Currentlength;
	int totalLength;
	LOCKING* KEY;
public:
	user();
	~user();
	void addLength(int);
	void init();
	void setTotal(int);
	void setCurrent(int);
	void setLock();
	void releaseLock();
	int getCurrent();
	int getTotal();
	char* getMessage();
};
