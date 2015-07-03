
#include "stdafx.h"


#pragma comment(lib, "Ws2_32.lib")


#define MaxClnt 30
#define BUF_SIZE 1024
void ErrorHandling(char *msg);



class Player{
public:

	enum
	{
		ID = 2
	};

	int size;
	char *p;
	Player(int n){
		this->size = n;
		this->p = new char[n + 1];
		p = { 0, };
	}
};



class NPC{
public:
	int size;
	int Ty;
	char name[100];
	NPC(){}
	/*NPC(int n, int Ty, char* input){
		this->size = n;
		this->Ty = Ty;
		p = new char[n + 1];
		strcpy(this->p, input);
		}*/
};
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


	fputs("INPUT message(Q to quit) : ", stdout);
	int h_send, sumof = 0;

	NPC test;
	int Sz = sizeof(NPC);
	memcpy(message, &Sz, sizeof(Sz));

	int Sz_2 = sizeof(NPC.Ty);
	memcpy(message + sizeof(Sz), )
	//int packetId = (int)Player.ID;
	//memcpy(message + sizeof(Sz), &packetId, sizeof(packetId));

	
	char input[] = "test";


//	h_send = send(hSocket, (char*)&test, sizeof(test), 0);
	

	
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
