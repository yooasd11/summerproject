// iocp_module.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")



int _tmain()
{
	//윈도우 소켓 시작
	ConnectSocket server;

	//컴플리션포트 생성, 스레드생성까지포함..
	Iocp CompletionPort;
	CompletionPort.makeObject();

	//해당 포트번호 생성 socket, bind 포함..
	server.Makelistener(9200);

	//listen 함수호출
	server.listenClient(5);

	while (1){
		SOCKET tempSock;
		ClientHandle tempHandle;

		//클라이언트소켓을 받음...
		if ((tempSock = server.acceptClient(tempHandle)) == INVALID_SOCKET) ErrorHandling("accept() error");
		tempHandle.ClntSock = tempSock;

		//클라이언트 맵핑, 몇 번 인덱스인지
		CompletionPort.cm->mapping(tempSock);

		//정보를 입력하기전 초기화
		tempHandle.Copy();


		//클라이언트 소켓 포트 등록
		CompletionPort.registerOjbect(tempHandle);

		//읽기모드 초기화
		tempHandle.ReadMode();

		//메시지 받음
		CompletionPort.RecvMessage(tempHandle);
	}
	return 0;
}

void ErrorHandling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}