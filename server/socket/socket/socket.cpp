// socket.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Locking.h"

#pragma comment(lib, "Ws2_32.lib")

#define MsgSize 100
#define NAME 20
#define MaxClnt 30
void ErrorHandling(char *msg);
void SendMsg(char* s, int );
unsigned WINAPI HandleClnt(void* s);



SOCKET clntSocks[MaxClnt];
LOCKING *KEY;
int ClntCount = 0;

//int argc, _TCHAR* argv[]
int _tmain()
{
	printf("SERVER\n");
	int portnumber;
	WSADATA swaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	HANDLE hSndThread, hRcvThread;
	int clntAdrSz;
	
	//printf("포트번호 입력\n");
	//scanf("%d", &portnumber);
	
	if (WSAStartup(MAKEWORD(2, 2), &swaData) != 0)
		ErrorHandling("WSASartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(9200);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) ErrorHandling("bind() error!");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	KEY = new LOCKING;

	while (1){
		clntAdrSz = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

		if (hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() error");

		KEY->LOCK();
		clntSocks[ClntCount++] = hClntSock;
		KEY->UNLOCK();

		hSndThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);

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


unsigned WINAPI HandleClnt(void *s){
	SOCKET clnt_sock = *((SOCKET*)s);
	int str_len = 0, i;
	char msg[MsgSize];

	while ((str_len = recv(clnt_sock, msg, sizeof(msg), 0)) != 0)
		SendMsg(msg, str_len);

	KEY->LOCK();
	for (i = 0; i < ClntCount; i++){
		if (clnt_sock == clntSocks[i]){
			while (i++ < ClntCount - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	ClntCount--;
	KEY->UNLOCK();
	closesocket(clnt_sock);
	return 0;
}

void SendMsg(char* s, int len){
	
	int i;
	KEY->LOCK();
	for (i = 0; i < ClntCount; i++)
		send(clntSocks[i], s, len, 0);
	KEY->UNLOCK();
	return;
}