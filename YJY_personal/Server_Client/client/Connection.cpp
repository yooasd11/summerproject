#include "Connection.h"
#include "HandlerMap.h"
#include "Character.h"

inline void Connection::ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int Connection::AccountTo(char* ip, int port){
	WSADATA wsaData;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
		ErrorHandling("WSAStartup() error");
		return -1;
	}

	if ((Socket = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		ErrorHandling("socket() error");
		return -2;
	}

	if (!strcmp(ip, "localhost")) strcpy(ip, "127.0.0.1");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(ip);
	servAdr.sin_port = htons(port);

	if (connect(Socket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR){
		ErrorHandling("connect() error");
		return -3;
	}
	fflush(stdin);
	FD_ZERO(&reads);
	FD_SET(Socket, &reads);
	printf("Connection established to %s:%d\n", ip, port);
	return 0;
}

bool Connection::RecvMsg(){
	char PktBuf[PKTLENGTH];
	Packet packet;

	int retRecv = recv(Socket, PktBuf, 2 * sizeof(USHORT), 0);
	if (retRecv == -1) return false;	// 소켓이A 끊어진 경우

	USHORT PktLen, PktType;

	memcpy(&PktLen, PktBuf, sizeof(USHORT));
	memcpy(&PktType, PktBuf + sizeof(USHORT), sizeof(USHORT));

	packet.setLength(PktLen);
	packet.setType(PktType);

	int rcvdPacketLength = PKTHEADERSIZE;
	int totalSize = PktLen + PKTHEADERSIZE;
	while (rcvdPacketLength < totalSize)
	{
		int retSuccessRevSize = recv(Socket, PktBuf + rcvdPacketLength, PktLen, 0);
		rcvdPacketLength += retSuccessRevSize;
	}

	packet.setMsg(PktBuf + PKTHEADERSIZE, PktLen);

	HandlerMap::GetInstance()->HandlePacket(packet);

	return true;
}

bool Connection::Receive(){
	while (1){
		cpyReads = reads;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1;

		int fdNum;

		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR) return false;
		if (fdNum <= 0) break;

		if (FD_ISSET(Socket, &cpyReads)){
			if (RecvMsg() == -1)
				ErrorHandling("Connection disabled");
		}
	}
	return true;
}


unsigned WINAPI Connection::sendMsg(void* s){
	char PktBuf[PKTLENGTH];
	USHORT PktLen, PktType;
	while (1){
		MakePacket(PktType, PktLen, PktBuf);

		int sndPacketLength = 0, totalSize = PKTHEADERSIZE + PktLen;
		while (sndPacketLength < totalSize){
			int retPktLength = send(this->GetSocket(), PktBuf + sndPacketLength, totalSize, 0);
			sndPacketLength += retPktLength;
		}
	}
	return 0;
}

void Connection::makePacket(USHORT& TypeBuf, USHORT& LengthBuf, char* PktBuf){
	fscanf_s(stdin, "%d", &TypeBuf);

	if (TypeBuf == P_ECHO){
		fscanf_s(stdin, "%s", PktBuf + PKTHEADERSIZE, PKTBODYSIZE);		//write a message on packet buffer directly

		LengthBuf = (sizeof(char)* strlen(PktBuf + PKTHEADERSIZE)) + sizeof(char);
	}

	else if (TypeBuf == P_ECHOLIST){
		int listBuf[PKTBODYSIZE / sizeof(int)];
		fscanf_s(stdin, "%d", &listBuf[0]);

		int listSize = listBuf[0];

		for (int i = 1; i <= listSize; ++i){
			if (i >= PKTBODYSIZE / sizeof(int)) break;
			scanf_s("%d", &listBuf[i]);
		}

		LengthBuf = sizeof(int)+(sizeof(int)* listSize);
		memcpy(PktBuf + PKTHEADERSIZE, listBuf, LengthBuf);
	}

	else if (TypeBuf == P_ECHOCHARACTER){
		char name[NAMESIZE];
		double x, y;
		long long id;
		fscanf_s(stdin, "%s", name, NAMESIZE);
		fscanf_s(stdin, "%lf %lf %lld", &x, &y, &id);

		LengthBuf = NAMESIZE + 2 * sizeof(double)+sizeof(long long);
		memcpy(PktBuf + PKTHEADERSIZE, name, NAMESIZE);
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE, &x, sizeof(double));
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE + sizeof(double), &y, sizeof(double));
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE + 2 * sizeof(double), &id, sizeof(long long));
	}

	memcpy(PktBuf, &LengthBuf, sizeof(USHORT));
	memcpy(PktBuf + sizeof(USHORT), &TypeBuf, sizeof(USHORT));
}