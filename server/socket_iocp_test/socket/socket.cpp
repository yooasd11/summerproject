// socket.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//자주 사용되는 기능 클래스로 묶기 

//비동기

// socket_clnt.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Locking.h"

//전처리구문 중 하나
#pragma comment(lib, "Ws2_32.lib")
#define BUF_SIZE 100
#define READ 3
#define WRITE 5


typedef struct{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode;

}PER_IO_DATA, *LPPER_IO_DATA;

class Player{
public:
	int size;
	char *p;
	Player(int n){
		this->size = n;
		this->p = new char[n+1];
		p = { 0, };
	}
};

class NPC{
public:
	int size;
	int Ty;
	char *p;
	NPC(int n){
		this->size = n;
		this->p = new char[n + 1];
	}
	NPC(int n, int Ty, char* input){
		this->size = n;
		this->Ty = Ty;
		p = new char[n + 1];
		strcpy(this->p, input);
	}
};

unsigned WINAPI EchoThreadMain(void* CompletionPortIO);
void ErrorHandling(char *msg);

int _tmain(){

	printf("server");
	WSADATA wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int i;
	DWORD flags = 0, recvBytes;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");


	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);

	for (i = 0; i < sysInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, (void*)hComPort, 0, NULL);


	hServSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	//make non-blocking model, mode == 1 -> "non-blocking", mode == 0 -> "blocking" 
	//ioctlsocket(hLisnSock, FIONBIO, &mode);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(9200);


	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR) ErrorHandling("listen() error");

	
	while (1){
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);


		//register client socket
		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)handleInfo, 0);

		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);
		
	}
	closesocket(hServSock);

	WSACleanup();
	return 0;
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

unsigned WINAPI EchoThreadMain(void* pComPort){
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1){
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);

		sock = handleInfo->hClntSock;
		
		if (ioInfo->rwMode == READ){
			puts("message received!");
			if (bytesTrans == 0){
				closesocket(sock);
				free(handleInfo); free(ioInfo);
				continue;
			}
			ioInfo->buffer[bytesTrans] = 0;
			NPC test(bytesTrans);
			

			printf("%d %s", bytesTrans, ioInfo->buffer);

			
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = bytesTrans;
			ioInfo->rwMode = WRITE;
			WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
		else{
			puts("Message sent!");
			free(ioInfo);
		}
	}
	return 0;
}