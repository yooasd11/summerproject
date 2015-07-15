#include "stdafx.h"

#include "ConnectSocket.h"


ConnectSocket::ConnectSocket()
{
	if (WSAStartup(MAKEWORD(2, 2), &(this->wsaData)) != 0)  ErrorHandling("WSAStartup error");

}


ConnectSocket::~ConnectSocket()
{
}

//소켓을 만들고 바인드까지
void ConnectSocket::Makelistener(int port){
	this->Sock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	memset(&(this->Adr), 0, sizeof(this->Adr));
	this->Adr.sin_family = AF_INET;
	this->Adr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->Adr.sin_port = htons(port);

	if (::bind(this->Sock, (SOCKADDR*)&this->Adr, sizeof(this->Adr)) == SOCKET_ERROR) ErrorHandling("bind() error");
	return;
}

SOCKET ConnectSocket::acceptClient(ClientHandle& client){
	return client.ClntSock = accept(this->Sock, (SOCKADDR*)&client.clntAdr, &(client.addrLen));
}

void ConnectSocket::listenClient(int num){
	if (listen(this->Sock, num) == SOCKET_ERROR) ErrorHandling("listen() error");
	return;
}

SOCKET ConnectSocket::getSocket(){
	return this->Sock;
}