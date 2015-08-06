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
	srand(time(NULL));
	
	serv.MakeListener(9200);
	serv.listenClient(5);
	CompletionPort.setServSock(serv.getSocket());
	//CompletionPort.GENERATE_NPC(3);

	while (1){
		SOCKET sock;
		int index;
		ClientHandle handle;
		if ((sock = serv.ConnectSocket(handle)) == INVALID_SOCKET) ErrorHandling("accept error");
		handle.ClntSock = sock;
		
		CompletionPort.Object_Manager->REGIST(Object_USER, sock);
		index = CompletionPort.Object_Manager->FIND_USER(sock);

		handle.Copy();
		CompletionPort.registerObject(handle);

		PacketHandler::GetInstance()->SendAccountPacket(sock,index);
		PacketHandler::GetInstance()->BroadCastAccountPacket();

		std::shared_ptr<USER> user= std::static_pointer_cast<USER>(CompletionPort.Object_Manager->FIND(index));
		TimerJob job;
		job.exectime = GetTickCount() + 30;
		job.func = std::bind(&USER::USER_MOVE, user);
		{
			LOCKING(&IocpConstructor::queueLock);
			IocpConstructor::jobs.push_back(job);
		}
		handle.ReadMode();	// 고민해볼것
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
