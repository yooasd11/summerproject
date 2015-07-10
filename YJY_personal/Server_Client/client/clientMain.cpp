#ifndef MAIN_INCLUDE
#define MAIN_INCLUDE
#include "winsockstd.h"
#include "Character.h"
#include "Packet.h"
#include "Connection.h"
#endif

Connection* ConnectionManager;

void MakePacket(USHORT& TypeBuf, USHORT& LengthBuf, char* Buf);
unsigned WINAPI SendMsg(void* s);

//Make a packet and send to server
unsigned WINAPI SendMsg(void* s){
	char PktBuf[PKTLENGTH];
	USHORT PktLen, PktType;
	while (1){
		MakePacket(PktType, PktLen, PktBuf);

		int sndPacketLength = 0, totalSize = PKTHEADERSIZE + PktLen;
		while (sndPacketLength < totalSize){
			int retPktLength = send(ConnectionManager->GetSocket(), PktBuf + sndPacketLength, totalSize, 0);
			sndPacketLength += retPktLength;
		}
	}
	return 0;
}

//Make a packet on Pktbuf
void MakePacket(USHORT& TypeBuf, USHORT& LengthBuf, char* PktBuf){
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



int _tmain()
{
	char ip[101] = "localhost";
	int port = 9200;

	/*printf_s("Input server info(IP PORT) : ");
	fscanf_s(stdin, "%s", ip, sizeof(ip));
	fscanf_s(stdin, "%d", &port);*/

	//Create a Connection Manager and account to server
	ConnectionManager = Connection::GetInstance();
	ConnectionManager->AccountTo(ip, port);

	//Start sending packet thread
	HANDLE hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, NULL, 0, NULL);

	while (true)
	{
		ConnectionManager->Receive();
	}

	WaitForSingleObject(hSndThread, INFINITE);

	return 0;
}
