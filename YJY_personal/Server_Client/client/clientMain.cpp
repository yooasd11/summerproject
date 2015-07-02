#include "winsockstd.h"
#include "Packet.h"

SOCKET clntAccount(char* ip, int port);
LOCKING* KEY;
bool IOStart(SOCKET& hSock);
inline void Disconnect(SOCKET& hSock){
	closesocket(hSock);
	WSACleanup();
	return;
}

void ErrorHandling(char *msg);
unsigned WINAPI SendMsg(void* s);
unsigned WINAPI RecvMsg(void* s);

int _tmain()
{
	char ip[101];
	int port;

	printf("Input server info(IP PORT) : ");
	scanf("%s %d", ip, &port);


	SOCKET hSock = clntAccount(ip, port);
	IOStart(hSock);
	Disconnect(hSock);

	return 0;
}


unsigned WINAPI RecvMsg(void* s){
	int strLen;
	char msg[PktBodySize];
	SOCKET Sock = *((SOCKET*)s);
	Packet packet;
	while (1){
		strLen = recv(Sock, (char*)&packet, PktLength, 0);
		if (strLen == -1) return -1;

		fputs(packet.getMsg(), stdout);
	}
	return 0;
}

unsigned WINAPI SendMsg(void* s){
	SOCKET Sock = *((SOCKET*)s);
	Packet packet;
	while (1){
		packet.inputMsg();
		packet.setType(ECHO);
		send(Sock, (char*)&packet, PktLength, 0);
	}
	return 0;
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

SOCKET clntAccount(char* ip, int port){
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAdr;
	KEY = new LOCKING;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
		ErrorHandling("WSAStartup() error");
		return NULL;
	}

	if ((hSock = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		ErrorHandling("socket() error");
		return NULL;
	}

	if (!strcmp(ip, "localhost")) strcpy(ip, "127.0.0.1");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(ip);
	servAdr.sin_port = htons(port);

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR){
		ErrorHandling("connect() error");
		return NULL;
	}
	fflush(stdin);
	printf("Connection established to %s:%d\n", ip, port);
	return hSock;
}

bool IOStart(SOCKET& hSock){
	if (hSock == NULL){
		ErrorHandling("Connection not established");
		return false;
	}
	HANDLE hSndThread, hRcvThread;

	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);

	
	return true;
}
