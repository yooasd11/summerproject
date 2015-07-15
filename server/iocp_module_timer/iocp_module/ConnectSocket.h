#pragma once

void ErrorHandling(char *msg);

class ConnectSocket
{
private:
	SOCKET Sock;
	SOCKADDR_IN Adr;
	WSADATA wsaData;

public:
	ConnectSocket();
	~ConnectSocket();
	void Makelistener(int);
	SOCKET acceptClient(ClientHandle& client);
	void listenClient(int);
	SOCKET getSocket();
};

