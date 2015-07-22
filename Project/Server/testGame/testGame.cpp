// testGame.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
		printf("%d ��������\n", sock);

		CompletionPort.cm->mapping(sock);

		handle.Copy();
		CompletionPort.registerObject(handle);

		PacketHandler::GetInstance()->SendAccountPacket(sock);
		PacketHandler::GetInstance()->BroadCastAccountPacket();

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
