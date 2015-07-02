#include "winsockstd.h"
#include "Packet.h"

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
	int port;
	printf("Input port number : ");
	scanf("%d", &port);

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
	SOCKET clnt_sock = *((SOCKET*)s);
	int str_len = 0, i;
	Packet packet;

	while ((str_len = recv(clnt_sock, (char*)&packet, PktLength, 0)) >= 0){
		USHORT pktLength = packet.getLength();
		USHORT pktType = packet.getType();
		printf("Packet received : length(%d), type(%d)\n", pktLength, pktType);
		if (pktType == ECHO)
			SendMsg((char*)&packet, str_len);
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