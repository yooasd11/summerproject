
#include "stdafx.h"


#pragma comment(lib, "Ws2_32.lib")


#define MaxClnt 30
#define BUF_SIZE 1024
void ErrorHandling(char *msg);

//int argc, _TCHAR* argv[]
int _tmain()
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAdr;

	WSABUF dataBuf;
	char message[BUF_SIZE];
	int strLen, readLen;

	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStarup() Error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&sendAdr, 0, sizeof(sendAdr));
	sendAdr.sin_family = AF_INET;
	sendAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sendAdr.sin_port = htons(9200);

	if (connect(hSocket, (SOCKADDR*)&sendAdr, sizeof(sendAdr)) == SOCKET_ERROR) ErrorHandling("Connect error!");

	while (1){
		fputs("INPUT message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break;

		strLen = strlen(message);
		send(hSocket, message, strLen, 0);
		readLen = 0;
		while (1){
			readLen += recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);
			if (readLen >= strLen) break;
		}
		message[strLen] = 0;
		printf("Message from server: %s", message);

	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
