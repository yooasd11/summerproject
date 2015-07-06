// socket_clnt.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "../test/addressbook.pb.h"
#include "../test/chatting.pb.h"

#pragma comment(lib, "Ws2_32.lib")

#define MsgSize 100
#define NAME 20

using namespace std;

void ErrorHandling(char *msg);
unsigned WINAPI SendMsg(void* s);
unsigned WINAPI RecvMsg(void* s);

char msg[MsgSize];

//int argc, _TCHAR* argv[]
int _tmain()
{
	printf("CLIENT\n");
	char Address[20];
	int portnumber;
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAdr;
	HANDLE hSndThread, hRcvThread;

	//printf("아이피, 포트번호 입력\n");
	//scanf("%s %d", Address, &portnumber);
	//printf("%s %d\n", Address, portnumber);

	//fflush(stdin);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(9200);

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);

	//추가된 부분
	CloseHandle(hSndThread);
	CloseHandle(hRcvThread);
	//

	closesocket(hSock);
	WSACleanup();
	return 0;
}


unsigned WINAPI RecvMsg(void* s){
	char nameMsg[MsgSize + NAME] = { 0, };
	int strLen;

	SOCKET Sock = *((SOCKET*)s);
	while (1){

		strLen = recv(Sock, nameMsg, MsgSize + NAME - 1, 0);
		if (strLen == -1) return -1;
		nameMsg[strLen] = 0;
		int messageSize, dataType;
		memcpy(&messageSize, nameMsg, sizeof(messageSize));
		memcpy(&dataType, nameMsg + sizeof(messageSize), sizeof(dataType));

		if (dataType == 1){
			chatting::letter temp;
			temp.ParseFromArray(nameMsg + sizeof(messageSize)+sizeof(dataType), messageSize);
			printf("messageSize dataType message x y\n", messageSize, dataType, temp.chat(), temp.x(), temp.y());
		}
	}
	return 0;
}

unsigned WINAPI SendMsg(void* s){
	SOCKET Sock = *((SOCKET*)s);
	char nameMsg[MsgSize + NAME] = { 0, };

	while (1){
		printf("메시지와 엔피시 종류, 좌표 값을 입력\n");

		char inputMessage[MsgSize];
		int dx, dy, dt;

		chatting::letter temp;
		scanf("%s %d %d %d", inputMessage, &dt, &dx, &dy);
		//printf("%s %d %d %d", inputMessage, dt, dx, dy);
		fflush(stdin);

		temp.set_chat(inputMessage);
		temp.set_x(dx);
		temp.set_y(dy);


		printf("%d\n", temp.ByteSize());

		int messageSize = temp.ByteSize();
		int dataType = dt;
		memcpy(nameMsg, &messageSize, sizeof(messageSize));
		memcpy(nameMsg + sizeof(messageSize), &dataType, sizeof(dataType));

		temp.SerializeToArray(nameMsg + sizeof(messageSize) + sizeof(dataType), temp.ByteSize());

		if (strcmp(msg, "q\n")==0 || strcmp(msg, "Q\n")==0){
			closesocket(Sock);
			return 0;
		}
	//	sprintf(nameMsg, "CLIENT %d: %s", (int)Sock, msg);
		send(Sock, nameMsg, sizeof(dataType)+sizeof(messageSize)+temp.ByteSize(), 0);
	}
	return 0;
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}