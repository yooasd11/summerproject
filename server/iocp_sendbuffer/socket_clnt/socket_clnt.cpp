
#include "stdafx.h"
#include "../test/character.pb.h"

#pragma comment(lib, "Ws2_32.lib")


#define MaxClnt 30
#define BUF_SIZE 1024
#define Player_ 1
#define NPC_ 2
#define Monster_ 3
#define NameSize 20
#define ChatSize (1<<19)-1
#define HEADERSIZE 8

void ErrorHandling(char *msg);

//int argc, _TCHAR* argv[]
int _tmain()
{
	//char message[ChatSize + NameSize + HEADERSIZE] = { 0, };

	//char message[ChatSize];
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAdr;

	WSABUF dataBuf;
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
	char *cBuffer = new char[ChatSize];
	while (1){
	
		int Ty, pkSize;
		printf("input type\n");
		scanf("%d", &Ty);
		fflush(stdin);
		if (Ty == Player_){
			Character::character temp;
			
			FILE *fp = fopen("input.txt", "r");
			fscanf(fp, "%s", cBuffer);
			printf("%d\n", ChatSize);
			cBuffer[ChatSize] = NULL;
			strLen = strlen(cBuffer);
			send(hSocket, cBuffer, ChatSize, 0);

			fclose(fp);
			/*char cName[NameSize] = { 0, };
			char cChat[ChatSize] = { 0, };
			scanf("%s %s", cName, cChat);
			temp.set_name(cName); temp.set_chat(cChat);

			printf("%s %s\n", cName, cChat);

			pkSize = temp.ByteSize();
			memcpy(message, &pkSize, sizeof(pkSize));
			memcpy(message + sizeof(pkSize), &Ty, sizeof(Ty));
			temp.SerializeToArray(message + sizeof(pkSize)+sizeof(Ty), pkSize);


			printf("send size : %d\n", sizeof(pkSize)+sizeof(Ty)+pkSize);
			send(hSocket, message, sizeof(pkSize)+sizeof(Ty)+pkSize, 0);
			printf("send size : %d\n", sizeof(pkSize)+sizeof(Ty)+pkSize);*/
		}
		/*else if (Ty == NPC_){
			Character::NPC temp;
			char cName[NameSize] = { 0, };
			int dx, dy = 0;
			scanf("%s %d %d", cName, &dx, &dy);
			temp.set_name(cName); temp.set_x(dx), temp.set_y(dy);

			pkSize = temp.ByteSize();
			memcpy(message, &pkSize, sizeof(pkSize));
			memcpy(message + sizeof(pkSize), &Ty, sizeof(Ty));
			temp.SerializeToArray(message + sizeof(pkSize)+sizeof(Ty), pkSize);

			send(hSocket, message, sizeof(pkSize)+sizeof(Ty)+pkSize, 0);
			}
			else if (Ty == Monster_){
			Character::Monster temp;
			char cName[NameSize] = { 0, };
			int p;
			scanf("%s %d", cName, &p);
			temp.set_name(cName), temp.set_power(p);

			pkSize = temp.ByteSize();
			memcpy(message, &pkSize, sizeof(pkSize));
			memcpy(message + sizeof(pkSize), &Ty, sizeof(Ty));
			temp.SerializeToArray(message + sizeof(pkSize)+sizeof(Ty), pkSize);

			send(hSocket, message, sizeof(pkSize)+sizeof(Ty)+pkSize, 0);

			}
			else{ printf("wrong input\n"); }*/
		readLen = 0;
		while (1){
			readLen += recv(hSocket, cBuffer, ChatSize, 0);
			if (readLen >= strLen) break;
		}
		cBuffer[strLen] = 0;
		printf("Message from server");
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
