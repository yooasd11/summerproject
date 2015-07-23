#pragma once


class IocpConstructor
{
private:
	SOCKET ServerSock;
	HANDLE ComPort;
	SYSTEM_INFO sysinfo;
	DWORD flags;
	DWORD recvBytes;
	Lock *queueLock;
	Lock *UserLock;
public:
	static GameManager *manageGame;
	static ClientManager *cm;
	static std::vector<TimerJob> jobs;
	
	IocpConstructor();
	~IocpConstructor();

	void registerObject(ClientHandle&);
	static unsigned WINAPI startThread(void*);

	void ThreadFunction();
	void RecvMessage(ClientHandle&);
	void SndMessage(ClientHandle&, SOCKET);

	void setServSock(SOCKET);
	void JobSchedule();
	HANDLE getPort();
	void closeSocket(SOCKET);

	
	void AutoNPC(int);

};

