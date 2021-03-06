#pragma once

#include "winsockstd.h"

class Connection{
private:
	SOCKET Socket;				//server socket
	inline void ErrorHandling(char* msg);

	bool RecvMsg();
	void makePacket(USHORT& TypeBuf, USHORT& LengthBuf, char* PktBuf);

	TIMEVAL timeout;
	fd_set reads, cpyReads;

	Connection() : Socket(NULL) {}
	~Connection() {
		closesocket(Socket);
		WSACleanup();
	}
public:
	static Connection* GetInstance(){
		static Connection pInstance;
		return &pInstance;
	}
	int AccountTo(char* ip, int port);
	SOCKET GetSocket(){
		return Socket;
	}
	bool Receive();
	unsigned WINAPI sendMsg(void* s);
};

