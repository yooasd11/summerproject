#include "stdafx.h"
#include "Iocp.h"


Iocp::Iocp()
{
	this->cm = new ClientManager;
	this->flags = 0;
	this->ComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&this->sysinfo);
	for (int i = 0; i < this->sysinfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, startThread, this, 0, NULL);
	return;
}
Iocp::~Iocp()
{
	delete this->cm;
}

//스레드 생성까지...
void Iocp::makeObject(){
	return;
}
void Iocp::registerOjbect(ClientHandle& client){
	CreateIoCompletionPort((HANDLE)client.ClntSock, this->ComPort, (DWORD)client.handleinfo, 0);
	return;

}

void Iocp::RecvMessage(ClientHandle& client){
	WSARecv(client.handleinfo->ClntSock, &(client.ioInfo->wsaBuf), 1, &(this->recvBytes), &(this->flags), &(client.ioInfo->overlapped), NULL);
	return;
}

void Iocp::SndMessage(ClientHandle& client, SOCKET sock){
	WSASend(sock, &(client.ioInfo->wsaBuf), 1, NULL, 0, &(client.ioInfo->overlapped), NULL);
	return;
}

unsigned WINAPI Iocp::startThread(void *th){
	Iocp *Thread = (Iocp*)th;
	Thread->ThreadFunction();
	return 0;
}

void Iocp::ThreadFunction(){

	SOCKET sock;
	HANDLE hComPort = (HANDLE)this->ComPort;
	ClientHandle tempHandle;

	while (1){
		GetQueuedCompletionStatus(hComPort, &(tempHandle.bytesTrans), (LPDWORD)&tempHandle.handleinfo,(LPOVERLAPPED*)&(tempHandle.ioInfo), INFINITE);
		sock = tempHandle.handleinfo->ClntSock;

		int index = this->cm->retIndex(sock);

		if (tempHandle.ioInfo->rwMode == READ){
			puts("Message Receive");
			if (tempHandle.bytesTrans == 0){
				closesocket(sock);
				delete tempHandle.handleinfo;
				delete tempHandle.ioInfo;
				continue;
			}
			this->cm->clientdata[index].setLock();
			this->cm->clientdata[index].setTotal(tempHandle.bytesTrans);
			printf("%d\n", tempHandle.bytesTrans);
			memcpy(this->cm->clientdata[index].getMessage(), tempHandle.ioInfo->wsaBuf.buf, tempHandle.bytesTrans);
			this->cm->clientdata[index].printMessage();
			this->cm->clientdata[index].releaseLock();

			tempHandle.WriteMode();
			this->SndMessage(tempHandle, sock);
			tempHandle.ReadMode();
			this->RecvMessage(tempHandle);
		}
		if (tempHandle.ioInfo->rwMode == WRITE){
			printf("메시지를 보냄\n");
		}
	}
	return;
}