#pragma once


class IocpConstructor
{
private:
	SOCKET ServerSock;
	HANDLE ComPort;
	SYSTEM_INFO sysinfo;
	DWORD flags;
	DWORD recvBytes;
	
	Lock *UserLock;
public:
	static GameManager *manageGame;
	static ClientManager *cm;
	static AIManager *nm;
	static int ObjectCount;
	static Lock* ObjectKey;
	static Lock* queueLock;

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

	
	void generateAI(int);

};

