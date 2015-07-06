// socket.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//자주 사용되는 기능 클래스로 묶기 

#include "stdafx.h"
#include "Locking.h"

#pragma comment(lib, "Ws2_32.lib")

#include "../test/addressbook.pb.h"
#include "../test/chatting.pb.h"

#define MsgSize 100
#define NAME 20
#define MaxClnt 30
void ErrorHandling(char *msg);
void SendMsg(char* s, int );
unsigned WINAPI HandleClnt(void* s);

using namespace std;

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

	//추가된 부분
	//하나의 스레드 핸들러 변수로 여러개 스레드 생성했는데 모두 종료될 때까지 기다리나...OR 위의 루프를 빠져나올수 없다..나올수있다면?
	WaitForSingleObject(hSndThread, INFINITE);
	
	//여기서 삭제되는 스레드 핸들러는?..
	CloseHandle(hSndThread);
	delete KEY;
	//


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
	char msg[MsgSize] = { 0, };

	//while ((str_len = recv(clnt_sock, msg, sizeof(msg), 0)) != 0)
	//	SendMsg(msg, str_len);

	str_len = recv(clnt_sock, msg, sizeof(msg), 0);
	if (str_len == 0) printf("error");
	else printf("%d\n", str_len);


	int messageSize, dataType;
	memcpy(&messageSize, msg, sizeof(messageSize));
	memcpy(&dataType, msg + sizeof(messageSize), sizeof(dataType));

	printf("%d\n", messageSize);

	if (dataType == 1){
		chatting::letter temp;
		char tempChat[MsgSize];

		temp.ParseFromArray(msg + sizeof(messageSize)+sizeof(dataType), messageSize);
		printf("messageSize : %d, dataType : %d, message : %s, x : %d, y : %d \n", messageSize, dataType, temp.chat().c_str(), temp.x(), temp.y());
	}

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