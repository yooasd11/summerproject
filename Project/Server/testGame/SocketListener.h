#pragma once
class SocketListener
{
private:
	SOCKET Sock;
	SOCKADDR_IN Adr;
	WSADATA wsaData;
public:
	SocketListener();
	~SocketListener();
	void MakeListener(int);
	void listenClient(int);
	SOCKET ConnectSocket(ClientHandle&);
	SOCKET getSocket();
};

