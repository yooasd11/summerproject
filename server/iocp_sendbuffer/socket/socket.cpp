#include "stdafx.h"
#include "../test/character.pb.h"

//��ó������ �� �ϳ�
#pragma comment(lib, "Ws2_32.lib")

//unsigned WINAPI EchoThreadMain(void* CompletionPortIO);
void ErrorHandling(char *msg);

int _tmain(){

	//��Ʈ��ȣ�� �Է¹޴´�. �����带 �������� bind �Լ����� ó����..
	Iocp iocp(9200);

	//listen �Լ� ȣ��
	iocp.ListenClient(5);

	//Ŭ���̾�Ʈ ������ �Է¹޾ƾ���...
	while (1){
		HandleClient Chandle;

		//Ŭ���̾�Ʈ�� �Է¹���..
		if (iocp.acceptClient(Chandle) == INVALID_SOCKET) ErrorHandling("accept() error");

		//Ŭ���̾�Ʈ ������ ���� iocp������ ó����..
		//single.mapping(Chandle.hClntSock);

		//�������� ����
		Chandle.copy();

		//��Ʈ���
		iocp.registerPort(Chandle);

		//�б���� �ʱ�ȭ
		Chandle.ReadMode();

		//�޽���ó��
		iocp.RecvMessage(Chandle);
	}
	return 0;
}

void ErrorHandling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}