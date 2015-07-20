// testGame.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "../protobuffer/AccountPacket.pb.h"
#include "../protobuffer/InGamePacket.pb.h"

#pragma comment(lib, "Ws2_32.lib")

int _tmain()
{
	SocketListener serv;
	IocpConstructor CompletionPort;

	serv.MakeListener(9200);
	serv.listenClient(5);
	CompletionPort.setServSock(serv.getSocket());
	//CompletionPort.AutoNPC(5);

	while (1){
		SOCKET sock;
		ClientHandle handle;
		if ((sock = serv.ConnectSocket(handle)) == INVALID_SOCKET) ErrorHandling("accept error");
		handle.ClntSock = sock;
		printf("%d 유저접속\n", sock);

		CompletionPort.cm->mapping(sock);

		handle.Copy();
		CompletionPort.registerObject(handle);

		//접속되면 최초로 메시지를 보냄...
		char buffer[500];
		AccountPacket::S_Account temp;
		//USER user = CompletionPort.cm

		unsigned short size = 0, ty = 0, len = 0;
		temp.set_uid(sock); temp.set_hp(100); temp.set_x(200), temp.set_y(200);
		size = temp.ByteSize();
		ty = ACCOUNT; // 
		memcpy(buffer, &size, sizeof(size));
		memcpy(buffer + sizeof(size), &ty, sizeof(ty));
		temp.SerializeToArray(buffer + sizeof(size)+sizeof(ty), temp.ByteSize());
		len = temp.ByteSize() + sizeof(size)+sizeof(ty);
		send(sock, buffer, len, 0);
		

		handle.ReadMode();
		CompletionPort.RecvMessage(handle);
	}
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
