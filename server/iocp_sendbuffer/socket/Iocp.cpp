#include "stdafx.h"
#include "Iocp.h"


Iocp::Iocp()
{

}

Iocp::Iocp(unsigned short port){
	this->ClientManager = new ManageClient;
	this->flags = 0;
	if (WSAStartup(MAKEWORD(2, 2), &(this->wsaData)) != 0)  ErrorHandling("WSAStartup error");
	this->ComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysinfo);

	for (int i = 0; i < sysinfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, this, 0, NULL);

	this->servSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(port);

	if (bind(this->servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) ErrorHandling("bind() error");
}

Iocp::~Iocp()
{
	delete this->ClientManager;
}

void Iocp::ListenClient(int num){
	if (listen(this->servSock, num) == SOCKET_ERROR) ErrorHandling("listen() error");
	return;
}

SOCKET Iocp::getServSock(){
	return this->servSock;
}

HANDLE Iocp::getHandle(){
	return this->ComPort;
}

SOCKET Iocp::acceptClient(HandleClient& client){
	client.hClntSock = accept(this->servSock, (SOCKADDR*)&client.clntAdr, &(client.addrLen));
	ClientManager->mapping(client.hClntSock);
	return client.hClntSock;
}

void Iocp::registerPort(HandleClient& client){
	CreateIoCompletionPort((HANDLE)client.hClntSock, this->ComPort, (DWORD)client.handleInfo, 0);
	return;
}

void Iocp::RecvMessage(HandleClient& client){
	WSARecv(client.handleInfo->hClntSock, &(client.ioInfo->wsaBuf), 1, &(this->recvBytes), &(this->flags), &(client.ioInfo->overlapped), NULL);
	return;
}

void Iocp::EndIocp(){
	closesocket(this->servSock);
	WSACleanup();
	return;
}

unsigned WINAPI Iocp::EchoThreadMain(void* th){
	Iocp *Thread = (Iocp*)th;
	Thread->startThread();
	return 0;
}

void Iocp::startThread(){
	HANDLE hComPort = (HANDLE)this->ComPort;

	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1){
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);

		sock = handleInfo->hClntSock;
		//printf("client : %d\n", sock);

		int index = this->ClientManager->retIndex(sock);
		//printf(" index : %d\n", index);


		if (ioInfo->rwMode == READ){
			printf("READ receive size: %d\n", bytesTrans);
			puts("message received!");
			if (bytesTrans == 0){
				closesocket(sock);
				delete handleInfo; delete ioInfo;
				continue;
			}
			ClientManager->us[index].setLock();
			ClientManager->us[index].setTotal(bytesTrans);
			memcpy(ClientManager->us[index].getMessage(), ioInfo->wsaBuf.buf, bytesTrans);
			printf("gettotal : %d\n", ClientManager->us[index].getTotal());
			ClientManager->us[index].releaseLock();
			/*else{
				int dataSize, Ty, rcvTotal = 0;
				memcpy(&dataSize, ioInfo->wsaBuf.buf, sizeof(dataSize));

				ClientManager->us[index].setLock();
				memcpy(ClientManager->us[index].getMessage(0), ioInfo->wsaBuf.buf + sizeof(dataSize), sizeof(Ty));
				memcpy(ClientManager->us[index].getMessage(sizeof(Ty)), ioInfo->wsaBuf.buf + sizeof(dataSize)+sizeof(Ty), bytesTrans - sizeof(dataSize)-sizeof(Ty));
				ClientManager->us[index].setTotal(dataSize + sizeof(Ty)+sizeof(dataSize));
				ClientManager->us[index].releaseLock();
			}*/
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = bytesTrans;
			ioInfo->rwMode = WRITE;
			WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			ioInfo = new PER_IO_DATA;
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buf;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
		else if(ioInfo->rwMode == WRITE){
			printf(" WRITE %d\n", bytesTrans);
			ClientManager->us[index].setLock();

			//다 보낸 경우
			if (ClientManager->us[index].getTotal() == ClientManager->us[index].getCurrent() + bytesTrans){
				ClientManager->us[index].init();
				printf("%d %d\n", ClientManager->us[index].getTotal(), ClientManager->us[index].getCurrent());
			}
			//못 보낸 경우
			else{
				ClientManager->us[index].addLength(bytesTrans);
				memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
				ioInfo->wsaBuf.len = ClientManager->us[index].getTotal() - ClientManager->us[index].getCurrent();
				memcpy(ioInfo->wsaBuf.buf, ClientManager->us[index].getMessage(), ClientManager->us[index].getTotal() - ClientManager->us[index].getCurrent());
				ioInfo->rwMode = WRITE;
				WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
			}
			ClientManager->us[index].releaseLock();
			delete ioInfo;
		}
	}
	return;
}

