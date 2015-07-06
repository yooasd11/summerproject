#include "winsockstd.h"

#ifndef CONNECTION_DEF
#define CONNECTION_DEF

class Connection{
private:
	SOCKET Socket;
	LOCKING* KEY;
	void ErrorHandling(char* msg);
	unsigned WINAPI SendMsg(void* s);
	char* MakePacket(USHORT* TypeBuf, USHORT* LengthBuf);
	void ClientPacketHandle(Packet& p);
	int RecvMsg();
	int fdNum;
	TIMEVAL timeout;
	fd_set reads, cpyReads;
	Connection() : Socket(NULL) {}
	~Connection() {
		closesocket(Socket);
		WSACleanup();
	}
public:
	static Connection* GetInstance(){
		static Connection pInstance;
		return &pInstance;
	}
	int AccountTo(char* ip, int port);
	SOCKET GetSocket(){
		return Socket;
	}
	bool Receive();
};


void Connection::ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int Connection::AccountTo(char* ip, int port){
	WSADATA wsaData;
	SOCKADDR_IN servAdr;
	KEY = new LOCKING;

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

int Connection::RecvMsg(){
	char PktBuf[PKTLENGTH];
	Packet packet;

	while (1){
		int retRecv = recv(Socket, PktBuf, 2 * sizeof(USHORT), 0);
		if (retRecv == -1) return -1;	// ������ ������ ���

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

		ClientPacketHandle(packet);
	}
	return 0;
}

void Connection::ClientPacketHandle(Packet& p)
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

		for (unsigned int i = 0; i < vec.size(); ++i){
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

bool Connection::Receive(){
	cpyReads = reads;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1;
	if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR) return false;

	if (FD_ISSET(Socket, &cpyReads)){
		if (RecvMsg() == -1)
			ErrorHandling("Connection disabled");
	}
	return true;
}

#endif