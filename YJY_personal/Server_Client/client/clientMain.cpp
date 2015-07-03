#include "winsockstd.h"
#include "Packet.h"
#include "Character.h"

SOCKET clntAccount(char* ip, int port);
LOCKING* KEY;
bool IOStart(SOCKET& hSock);

void clntPktHandle(Packet& p);

inline void Disconnect(SOCKET& hSock){
	closesocket(hSock);
	WSACleanup();
	return;
}

void ErrorHandling(char *msg);
unsigned WINAPI SendMsg(void* s);
unsigned WINAPI RecvMsg(void* s);
char* makePacket(USHORT* TypeBuffer, USHORT* LengthBuffer);

int _tmain()
{
	char ip[101];
	int port;

	printf("Input server info(IP PORT) : ");
	fscanf(stdin, "%s %d", ip, &port);
	fflush(stdin);

	SOCKET hSock = clntAccount(ip, port);
	IOStart(hSock);
	Disconnect(hSock);

	return 0;
}


unsigned WINAPI RecvMsg(void* s){
	SOCKET servSock = *((SOCKET*)s);

	char PktBuf[PKTLENGTH];
	Packet packet;

	while (1){
		int retRecv = recv(servSock, PktBuf, 2 * sizeof(USHORT), 0);
		if (retRecv == -1) return -1;	// 소켓이 끊어진 경우

		USHORT PktLen, PktType;

		memcpy(&PktLen, PktBuf, sizeof(USHORT));
		memcpy(&PktType, PktBuf + sizeof(USHORT), sizeof(USHORT));

		packet.setLength(PktLen);
		packet.setType(PktType);

		int rcvdPacketLength = PKTHEADERSIZE;
		int totalSize = PktLen + PKTHEADERSIZE;
		while (rcvdPacketLength < totalSize)
		{
			int retSuccessRevSize = recv(servSock, PktBuf + rcvdPacketLength, PktLen, 0);
			rcvdPacketLength += retSuccessRevSize;
		}
		
		packet.setMsg(PktBuf + PKTHEADERSIZE, PktLen);

		clntPktHandle(packet);
	}

	return 0;
}

unsigned WINAPI SendMsg(void* s){
	SOCKET servSock = *((SOCKET*)s);
	char* PktBuf;

	while (1){
		USHORT PktLen, PktType;
		PktBuf = makePacket(&PktType, &PktLen);

		int sndPacketLength = 0, totalSize = PKTHEADERSIZE + PktLen;
		while (sndPacketLength < totalSize){
			int retPktLength = send(servSock, PktBuf + sndPacketLength, totalSize, 0);
			sndPacketLength += retPktLength;
		}
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

void clntPktHandle(Packet& p)
{
	USHORT PktType = p.getType();
	char* PktBody = p.getMsg();

	if (PktType == ECHO){
		fprintf(stdout, "%s\n", PktBody);
	}

	else if (PktType == ECHOLIST){
		int size = -1;

		memcpy(&size, PktBody, sizeof(int));

		std::vector<int> vec;

		for (int i = 1; i <= size; ++i){
			int val;
			memcpy(&val, PktBody + (sizeof(int)*i), sizeof(int));
			vec.push_back(val);
		}

		for (int i = 0; i < vec.size(); ++i){
			fprintf(stdout, "%d ", vec[i]);
		}
		fprintf(stdout, "\n");
	}

	else if (PktType == ECHOCHARACTER){
		Character character;

		memcpy(character.name, PktBody, NAMESIZE);
		memcpy(&character.x, PktBody + NAMESIZE, sizeof(double));
		memcpy(&character.y, PktBody + NAMESIZE + sizeof(double), sizeof(double));
		memcpy(&character.id, PktBody + NAMESIZE + 2 * sizeof(double), sizeof(long long));

		fprintf(stdout, "%s %lf %lf %lld\n", character.name, character.x, character.y, character.id);
	}
}

char* makePacket(USHORT* TypeBuffer, USHORT* LengthBuffer){
	char PktBuf[PKTLENGTH];
	fscanf(stdin, "%d", TypeBuffer);

	if (*TypeBuffer == ECHO){
		fscanf(stdin, "%s", PktBuf + PKTHEADERSIZE);	//패킷 버퍼의 바디에 직접 입력

		*LengthBuffer = (sizeof(char)* strlen(PktBuf + PKTHEADERSIZE)) + sizeof(char);
	}

	else if (*TypeBuffer == ECHOLIST){
		int listBuf[PKTBODYSIZE / sizeof(int)];
		fscanf(stdin, "%d", &listBuf[0]);

		int listSize = listBuf[0];

		for (int i = 1; i <= listSize; ++i){
			if (i >= PKTBODYSIZE / sizeof(int)) break;
			scanf("%d", &listBuf[i]);
		}

		*LengthBuffer = sizeof(int) + (sizeof(int)* listSize);
		memcpy(PktBuf + PKTHEADERSIZE, listBuf, *LengthBuffer);
	}

	else if (*TypeBuffer == ECHOCHARACTER){
		Character character;

		char name[NAMESIZE];
		double x, y;
		long long id;
		scanf("%s %lf %lf %lld", name, &x, &y, &id);

		*LengthBuffer = NAMESIZE + 2 * sizeof(double)+sizeof(long long);
		memcpy(PktBuf + PKTHEADERSIZE, name, NAMESIZE);
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE, &x, sizeof(double));
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE + sizeof(double), &y, sizeof(double));
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE + 2 * sizeof(double), &id, sizeof(long long));
	}

	memcpy(PktBuf, LengthBuffer, sizeof(USHORT));
	memcpy(PktBuf + sizeof(USHORT), TypeBuffer, sizeof(USHORT));
	return PktBuf;
}