#pragma once
class Iocp
{
private:
	HANDLE ComPort;
	SYSTEM_INFO sysinfo;
	DWORD flags;
	DWORD recvBytes;
public:
	ClientManager* cm;
	Iocp();
	~Iocp();
	void makeObject();
	void registerOjbect(ClientHandle&);
	static unsigned WINAPI startThread(void *th);
	void ThreadFunction();
	void RecvMessage(ClientHandle&);
	void SndMessage(ClientHandle&, SOCKET);

};

