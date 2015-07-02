// socket.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//자주 사용되는 기능 클래스로 묶기 

//비동기

// socket_clnt.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


//전처리구문 중 하나
#pragma comment(lib, "Ws2_32.lib")

#define BUF_SIZE 1024

void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(char *message);

WSABUF dataBuf;
char buf[BUF_SIZE];

DWORD recvBytes = 0;

int _tmain(){

	printf("SERVER\n");
	WSADATA wsaData;
	SOCKET hLisnSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;

	WSAOVERLAPPED overlapped;
	WSAEVENT evObj;

	int idx, recvAdrSz;
	DWORD flags = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&lisnAdr, 0, sizeof(lisnAdr));
	lisnAdr.sin_family = AF_INET;
	lisnAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	lisnAdr.sin_port = htons(9200);

	if (bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR) ErrorHandling("bind() error");

	if (listen(hLisnSock, 5) == SOCKET_ERROR) ErrorHandling("listen() error");

	recvAdrSz = sizeof(recvAdr);
	hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);

	if (hRecvSock == INVALID_SOCKET) ErrorHandling("accept() error");

	memset(&overlapped, 0, sizeof(overlapped));
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;
	evObj = WSACreateEvent();

	if (WSARecv(hRecvSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, CompRoutine) == SOCKET_ERROR){
		if (WSAGetLastError() == WSA_IO_PENDING) puts("Background data receive");

	}
	
	//idx = WSAWaitForMultipleEvents(1, &evObj, FALSE, WSA_INFINITE, TRUE);
	SleepEx(1000, true);
	//if (idx == WAIT_IO_COMPLETION) puts("Overlapped I/O Completed");
	//else ErrorHandling("WSARecv() error");

	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hLisnSock);
	WSACleanup();
	return 0;
}

void CALLBACK CompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags){
	if (dwError != 0) ErrorHandling("CompRoutine error");
	else{
		recvBytes = szRecvBytes;
		printf("Received message: %s \n", buf);
	}
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}