#pragma once
class ClientData
{
private:
	Buffer buffer;
	Lock* Key;
public:
	ClientData();
	~ClientData();
	void setLock();
	void releaseLock();
	void addLength(int);
	void init();
	void setTotal(int);
	void setCurrent(int);
	int getCurrent();
	int getTotal();
	char* getMessage();
	void printMessage();

};

