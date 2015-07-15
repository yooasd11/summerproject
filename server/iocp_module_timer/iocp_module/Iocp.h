#pragma once
class Iocp
{
private:
	SOCKET ServerSock;
	HANDLE ComPort;
	SYSTEM_INFO sysinfo;
	DWORD flags;
	DWORD recvBytes;
	Lock *queueLock;
	
public:
	GameManager *manageGame;


	std::queue<TimerJob> jobs;
	ClientManager* cm;
	Iocp();
	~Iocp();
	void makeObject();
	void registerOjbect(ClientHandle&);
	static unsigned WINAPI startThread(void *th);

	void ThreadFunction();
	void RecvMessage(ClientHandle&);
	void SndMessage(ClientHandle&, SOCKET);

	void setServerSock(SOCKET);
	HANDLE getport();

	void initTimer();
};

