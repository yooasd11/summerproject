#include "stdafx.h"
#include "SocketListener.h"


SocketListener::SocketListener()
{
	if (WSAStartup(MAKEWORD(2, 2), &(this->wsaData)) != 0) ErrorHandling("WSAStartup Error");
}


SocketListener::~SocketListener()
{
}


void SocketListener::MakeListener(int port)
{
	bool opt = true;
	this->Sock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	setsockopt(this->Sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));

	memset(&(this->Adr), 0, sizeof(this->Adr));
	this->Adr.sin_family = AF_INET;
	this->Adr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->Adr.sin_port = htons(port);

	if (::bind(this->Sock, (SOCKADDR*)&this->Adr, sizeof(this->Adr)) == SOCKET_ERROR) ErrorHandling("bind() error");
	return;
}

void SocketListener::listenClient(int num)
{
	if (listen(this->Sock, num) == SOCKET_ERROR) ErrorHandling("listen() error");
	return;
}

SOCKET SocketListener::ConnectSocket(ClientHandle& client)
{
	return client.ClntSock = accept(this->Sock, (SOCKADDR*)&client.clntAdr, &(client.addrLen));
}

SOCKET SocketListener::getSocket()
{
	return this->Sock;
}