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
public:
	static GameManager *manageGame;
	static ClientManager *cm;
	static std::vector<TimerJob> jobs;
	//GameManager *manageGame;
	//ClientManager *cm;

	//static GameManager manageGame;
	//static ClientManager cm;

	
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


	//
	void AutoNPC(int);

};

