// iocp_module.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")



int _tmain()
{
	//������ ���� ����
	ConnectSocket server;

	//���ø�����Ʈ ����, �����������������..
	Iocp CompletionPort;
	CompletionPort.makeObject();

	//�ش� ��Ʈ��ȣ ���� socket, bind ����..
	server.Makelistener(9200);

	//listen �Լ�ȣ��
	server.listenClient(5);

	while (1){
		SOCKET tempSock;
		ClientHandle tempHandle;

		//Ŭ���̾�Ʈ������ ����...
		if ((tempSock = server.acceptClient(tempHandle)) == INVALID_SOCKET) ErrorHandling("accept() error");
		tempHandle.ClntSock = tempSock;

		//Ŭ���̾�Ʈ ����, �� �� �ε�������
		CompletionPort.cm->mapping(tempSock);

		//������ �Է��ϱ��� �ʱ�ȭ
		tempHandle.Copy();


		//Ŭ���̾�Ʈ ���� ��Ʈ ���
		CompletionPort.registerOjbect(tempHandle);

		//�б��� �ʱ�ȭ
		tempHandle.ReadMode();

		//�޽��� ����
		CompletionPort.RecvMessage(tempHandle);
	}
	return 0;
}

void ErrorHandling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}