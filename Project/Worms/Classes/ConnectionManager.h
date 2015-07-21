#pragma once

#include "winsockstd.h"

class ConnectionManager{
private:
	SOCKET m_servSocket;				//server socket
	inline void errorHandling(char* msg);

	//IO function
	int recvMsg();

	//values for select()
	TIMEVAL m_timeout;
	fd_set m_reads, m_cpyReads;

	ConnectionManager() : m_servSocket(NULL) {}
	~ConnectionManager() {
		closesocket(m_servSocket);
		WSACleanup();
	}
public:
	static ConnectionManager* getInstance(){
		static ConnectionManager instance;
		return &instance;
	}
	int accountTo(char* ip, int port);
	SOCKET getSocket(){
		return m_servSocket;
	}
	bool receive();
	unsigned WINAPI transmit(const USHORT& PacketLenght, const USHORT& PacketType, void* s);
};

