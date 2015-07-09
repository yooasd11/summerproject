#include "winsockstd.h"
#include "Packet.h"
#include "Character.h"

void ErrorHandling(char *msg);
void SendMsg(char* s, int);
unsigned WINAPI HandleClnt(void* s);

SOCKET clntSocks[MaxClnt];
LOCKING *KEY;
int ClntCount = 0;
SOCKET servAccount(int port);
bool Listener(SOCKET& hServSock);


inline void Disconnect(SOCKET& hSock){
	closesocket(hSock);
	WSACleanup();
	return;
}

int _tmain()
{
	int port = 9200;;
	//printf_s("Input port number : ");
	//scanf_s("%d", &port);

	SOCKET servSock = servAccount(port);
	Listener(servSock);
	Disconnect(servSock);

	return 0;
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}


unsigned WINAPI HandleClnt(void *s){
	SOCKET clntSock = *((SOCKET*)s);
	int str_len = 0, i;

	char PktBuf[PKTLENGTH];

	while (1){
		int retRecv = recv(clntSock, PktBuf, 2 * sizeof(USHORT), 0);
		if (retRecv == -1) return -1;	// 소켓이 끊어진 경우

		USHORT PktLen, PktType;

		memcpy(&PktLen, PktBuf, sizeof(USHORT));
		memcpy(&PktType, PktBuf + sizeof(USHORT), sizeof(USHORT));

		printf("Packet <length(%d), type(%d)> received\n", PktLen, PktType);

		int rcvdPacketLength = PKTHEADERSIZE;
		int totalSize = PKTHEADERSIZE + PktLen;
		while (rcvdPacketLength < totalSize)
		{
			int retSuccessRevSize = recv(clntSock, PktBuf + rcvdPacketLength, PktLen, 0);
			rcvdPacketLength += retSuccessRevSize;
		}

		memcpy(&PktLen, PktBuf, sizeof(USHORT));
		memcpy(&PktType, PktBuf + sizeof(USHORT), sizeof(USHORT));


		SendMsg(PktBuf, totalSize);
	}

	KEY->LOCK();
	for (i = 0; i < ClntCount; i++){
		if (clntSock == clntSocks[i]){
			while (i++ < ClntCount - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	ClntCount--;
	KEY->UNLOCK();
	closesocket(clntSock);
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

SOCKET servAccount(int port){
	WSADATA swaData;
	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	KEY = new LOCKING;

	if (WSAStartup(MAKEWORD(2, 2), &swaData) != 0){
		ErrorHandling("WSASartup() error!");
		return NULL;
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(9200);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR){
		ErrorHandling("bind() error!");
		return NULL;
	}

	if (listen(hServSock, 5) == SOCKET_ERROR){
		ErrorHandling("listen() error!");
		return NULL;
	}
	printf("Server socket esatbished : port %d\n", port);
	return hServSock;
}

bool Listener(SOCKET& hServSock){
	HANDLE hSndThread, hRcvThread;

	int clntAdrSz;
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;

	while (1){
		clntAdrSz = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

		if (hClntSock == INVALID_SOCKET){
			ErrorHandling("accept() error");
			continue;
		}

		KEY->LOCK();
		clntSocks[ClntCount++] = hClntSock;
		KEY->UNLOCK();

		printf("Client %s:%d connected\n", inet_ntoa(clntAdr.sin_addr), clntAdr.sin_port);

		hSndThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);

	}
	return true;
}
