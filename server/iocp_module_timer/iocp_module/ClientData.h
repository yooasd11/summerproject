#pragma once
class ClientData
{
private:
	Buffer buffer;
	Lock *Key;
public:
	ClientData();
	~ClientData();
	void setLock();
	void releaseLock();
	//void ReadLock();
	//void WriteLock();
	//void ReleaseRead();
	//void ReleaseWrite();
	void addLength(int);
	void init();
	void setTotal(int);
	void setCurrent(int);
	int getCurrent();
	int getTotal();
	char* getMessage(int);
	void printMessage();

};

