#pragma once
class HandleClient
{
public:
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;
	int addrLen;

	HandleClient();
	~HandleClient();

	void setClntSock(SOCKET);
	void copy();
	void ReadMode();
};

