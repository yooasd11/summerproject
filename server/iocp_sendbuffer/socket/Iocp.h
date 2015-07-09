#pragma once

#include "HandleClient.h"

unsigned WINAPI EchoThreadMain(void* CompletionPortIO);
void ErrorHandling(char *msg);

class Iocp
{
private:
	SOCKET servSock;
	SOCKADDR_IN servAdr;
	SYSTEM_INFO sysinfo;
	WSADATA wsaData;
	HANDLE ComPort;
	DWORD flags, recvBytes;
	ManageClient *ClientManager;
public:
	Iocp();
	Iocp(unsigned short);

	~Iocp();
	void ListenClient(int);
	SOCKET getServSock();
	HANDLE getHandle();
	SOCKET acceptClient(HandleClient&);
	void registerPort(HandleClient&);
	void RecvMessage(HandleClient&);
	void EndIocp();
	static unsigned WINAPI EchoThreadMain(void* th);
	void startThread();
};

