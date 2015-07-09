#include "stdafx.h"
#include "../test/character.pb.h"

//전처리구문 중 하나
#pragma comment(lib, "Ws2_32.lib")

//unsigned WINAPI EchoThreadMain(void* CompletionPortIO);
void ErrorHandling(char *msg);

int _tmain(){

	//포트번호를 입력받는다. 스레드를 생성부터 bind 함수까지 처리함..
	Iocp iocp(9200);

	//listen 함수 호출
	iocp.ListenClient(5);

	//클라이언트 정보를 입력받아야함...
	while (1){
		HandleClient Chandle;

		//클라이언트를 입력받음..
		if (iocp.acceptClient(Chandle) == INVALID_SOCKET) ErrorHandling("accept() error");

		//클라이언트 소켓을 맵핑 iocp내에서 처리함..
		//single.mapping(Chandle.hClntSock);

		//소켓정보 복사
		Chandle.copy();

		//포트등록
		iocp.registerPort(Chandle);

		//읽기모드로 초기화
		Chandle.ReadMode();

		//메시지처리
		iocp.RecvMessage(Chandle);
	}
	return 0;
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}