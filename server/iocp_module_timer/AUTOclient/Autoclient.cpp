#define _WINSOCKAPI_
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <WinSock2.h>
#include <string>
#include <ctime>
#include "../character.pb.h"

#define MaxClnt 30
#define BUF_SIZE 1024
#define Player_ 0
#define NPC_ 1
#define Monster_ 2
#define kindOf 3
#define NameSize 20
#define ChatSize (1<<15)-1
#define HEADERSIZE 8

#pragma comment(lib, "Ws2_32.lib")
void ErrorHandling(char *msg);
unsigned WINAPI ThreadFunction(void* t);

int _tmain()
{
	char message[ChatSize];
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAdr;

	WSABUF dataBuf;
	int strLen, readLen;

	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStarup() Error");
	srand(time(0));
	for (int i = 0; i < 10; i++)
		_beginthreadex(NULL, 0, ThreadFunction, (void*)&i, 0, NULL);
	

		

	while (1){ }
	return 0;
}


unsigned WINAPI ThreadFunction(void* t){

	char *message = new char[ChatSize];
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAdr;
	WSABUF dataBuf;
	int strLen, readLen;
	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	int add = *((int*)t);

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&sendAdr, 0, sizeof(sendAdr));
	sendAdr.sin_family = AF_INET;
	sendAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sendAdr.sin_port = htons(9200);

	if (connect(hSocket, (SOCKADDR*)&sendAdr, sizeof(sendAdr)) == SOCKET_ERROR) ErrorHandling("Connect error!");
	
	int addd=add;
	
	while (1){
		memset(message, 0, sizeof(message));
		Sleep(rand()%10000);
		int Ty, pkSize, menu;
		
		Ty = rand()% 3;
		if (Ty == Player_){

			int temp1, temp2;
			Character::Player temp;
			int namesize = rand() % 10 + 3;
			int chatsize = rand() % 100 + 5;

			std::string cName, cChat;
			for (int i = 0; i < namesize; i++)
				cName += "a";
			for (int i = 0; i < chatsize; i++)
				cChat += "a";

			temp.set_name(cName); temp.set_chat(cChat);
			printf("%s %s \n", cName.c_str(), cChat.c_str());
			pkSize = temp.ByteSize();
			memcpy(message, &pkSize, sizeof(pkSize));
			memcpy(message + sizeof(pkSize), &Ty, sizeof(Ty));
			
			temp.SerializeToArray(message + sizeof(pkSize)+sizeof(Ty), pkSize);
			send(hSocket, message, sizeof(pkSize)+sizeof(Ty)+pkSize, 0);

			//실험구간//////////////////////
			memcpy(&temp1, message, sizeof(temp1));
			memcpy(&temp2, message + sizeof(temp2), sizeof(temp2));
			printf("size : %d, Type : %d\n", temp1, temp2);
			///////////////////

			strLen = sizeof(pkSize)+sizeof(Ty)+pkSize;
		}
		else if (Ty == NPC_){

			Character::NPC temp;
			int namesize = rand() % 10 + 5;
			std::string cName;
			int dx, dy = 0;
			for (int i = 0; i < namesize; i++) cName += "a";
			dx = rand() % 400; dy = rand() % 400;
			temp.set_name(cName); temp.set_x(dx), temp.set_y(dy);

			pkSize = temp.ByteSize();
			memcpy(message, &pkSize, sizeof(pkSize));
			memcpy(message + sizeof(pkSize), &Ty, sizeof(Ty));
			temp.SerializeToArray(message + sizeof(pkSize)+sizeof(Ty), pkSize);

			
			send(hSocket, message, sizeof(pkSize)+sizeof(Ty)+pkSize, 0);
			strLen = sizeof(pkSize)+sizeof(Ty)+pkSize;
		}
		else if (Ty == Monster_){
			Character::Monster temp;
			int namesize = rand() % 10 + 5;
			int p;
			
			std::string cName;
			for (int i = 0; i < namesize; i++) cName += "a";
			p = rand() % 400;
			temp.set_name(cName), temp.set_power(p);

			pkSize = temp.ByteSize();
			memcpy(message, &pkSize, sizeof(pkSize));
			memcpy(message + sizeof(pkSize), &Ty, sizeof(Ty));
			temp.SerializeToArray(message + sizeof(pkSize)+sizeof(Ty), pkSize);

			send(hSocket, message, sizeof(pkSize)+sizeof(Ty)+pkSize, 0);
			strLen = sizeof(pkSize)+sizeof(Ty)+pkSize;
		}
		readLen = 0;
		while (1){
			readLen += recv(hSocket, message, ChatSize, 0);
			if (readLen >= strLen) break;
		}
		message[strLen] = 0;
		printf("Message from server\n");
	}
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}