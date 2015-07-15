#pragma once
class ClientHandle
{
public:
	SOCKET ClntSock;
	SOCKADDR_IN clntAdr;
	IoData *ioInfo;
	HandleData *handleinfo;
	DWORD flags, bytesTrans;
	int addrLen;

	ClientHandle();
	~ClientHandle();
	void Copy();
	void ReadMode();
	void WriteMode();
};

