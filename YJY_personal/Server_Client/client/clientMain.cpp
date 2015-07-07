#include "winsockstd.h"
#include "Character.h"
#include "Packet.h"
#include "Connection.h"

LOCKING* KEY;
Connection* ConnectionManager;

void MakePacket(USHORT* TypeBuf, USHORT* LengthBuf, char* Buf);

unsigned WINAPI SendMsg(void* s){
	char PktBuf[PKTLENGTH];

	while (1){
		USHORT PktLen, PktType;
		MakePacket(&PktType, &PktLen, PktBuf);

		int sndPacketLength = 0, totalSize = PKTHEADERSIZE + PktLen;
		while (sndPacketLength < totalSize){
			int retPktLength = send(ConnectionManager->GetSocket(), PktBuf + sndPacketLength, totalSize, 0);
			sndPacketLength += retPktLength;
		}
	}
	return 0;
}

void MakePacket(USHORT* TypeBuf, USHORT* LengthBuf, char* PktBuf){
	fscanf_s(stdin, "%d", TypeBuf);

	if (*TypeBuf == ECHO){
		fscanf_s(stdin, "%s", PktBuf + PKTHEADERSIZE, PKTBODYSIZE);	//패킷 버퍼의 바디에 직접 입력

		*LengthBuf = (sizeof(char)* strlen(PktBuf + PKTHEADERSIZE)) + sizeof(char);
	}

	else if (*TypeBuf == ECHOLIST){
		int listBuf[PKTBODYSIZE / sizeof(int)];
		fscanf_s(stdin, "%d", &listBuf[0]);

		int listSize = listBuf[0];

		for (int i = 1; i <= listSize; ++i){
			if (i >= PKTBODYSIZE / sizeof(int)) break;
			scanf_s("%d", &listBuf[i]);
		}

		*LengthBuf = sizeof(int)+(sizeof(int)* listSize);
		memcpy(PktBuf + PKTHEADERSIZE, listBuf, *LengthBuf);
	}

	else if (*TypeBuf == ECHOCHARACTER){
		char name[NAMESIZE];
		double x, y;
		long long id;
		fscanf_s(stdin, "%s", name, NAMESIZE);
		fscanf_s(stdin, "%lf %lf %lld", &x, &y, &id);

		*LengthBuf = NAMESIZE + 2 * sizeof(double)+sizeof(long long);
		memcpy(PktBuf + PKTHEADERSIZE, name, NAMESIZE);
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE, &x, sizeof(double));
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE + sizeof(double), &y, sizeof(double));
		memcpy(PktBuf + PKTHEADERSIZE + NAMESIZE + 2 * sizeof(double), &id, sizeof(long long));
	}

	memcpy(PktBuf, LengthBuf, sizeof(USHORT));
	memcpy(PktBuf + sizeof(USHORT), TypeBuf, sizeof(USHORT));
}



int _tmain()
{
	char ip[101] = "localhost";
	int port = 9200;

	/*printf_s("Input server info(IP PORT) : ");
	fscanf_s(stdin, "%s", ip, sizeof(ip));
	fscanf_s(stdin, "%d", &port);*/

	ConnectionManager = Connection::GetInstance();
	ConnectionManager->AccountTo(ip, port);

	HANDLE hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, NULL, 0, NULL);

	while (true)
	{
		ConnectionManager->Receive();
	}

	WaitForSingleObject(hSndThread, INFINITE);

	return 0;
}
