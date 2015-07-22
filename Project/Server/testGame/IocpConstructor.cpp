#include "stdafx.h"
#include "PacketHandler.h"
#include "IocpConstructor.h"
#include "../protobuffer/AccountPacket.pb.h"
#include "../protobuffer/InGamePacket.pb.h"

//�����ϴ� ���
//HandlerMap::GetInstance()->HandlePacket(packet);


//'static'���� ������ ��� 'cpp'�� �ٽ� ���� ���־�� �Ѵ�.
GameManager* IocpConstructor::manageGame;
ClientManager* IocpConstructor::cm;
std::vector<TimerJob> IocpConstructor::jobs;

IocpConstructor::IocpConstructor()
{
	this->manageGame = new GameManager;
	this->cm = new ClientManager;
	this->queueLock = new Lock;
	this->flags = 0;
	this->ComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	
	GetSystemInfo(&this->sysinfo);
	for (int i = 0; i < this->sysinfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, startThread, this, 0, NULL);
	return;
}


IocpConstructor::~IocpConstructor()
{
	delete this->cm;
	delete this->manageGame;
	delete this->queueLock;
}

void IocpConstructor::registerObject(ClientHandle& client)
{
	CreateIoCompletionPort((HANDLE)client.ClntSock, this->ComPort, (DWORD)client.handleinfo, 0);
	return;
}

void IocpConstructor::RecvMessage(ClientHandle& client)
{
	WSARecv(client.handleinfo->ClntSock, &(client.ioinfo->wsaBuf), 1, &(this->recvBytes), &(this->flags), &(client.ioinfo->overlapped), NULL);
	return;
}

void IocpConstructor::SndMessage(ClientHandle& client, SOCKET sock)
{
	WSASend(sock, &(client.ioinfo->wsaBuf), 1, NULL, 0, &(client.ioinfo->overlapped), NULL);
	return;
}

unsigned WINAPI IocpConstructor::startThread(void* th)
{
	IocpConstructor* Thread = (IocpConstructor*)th;
	srand(time(NULL));
	Thread->ThreadFunction();
	return 0;
}

void IocpConstructor::setServSock(SOCKET s)
{
	this->ServerSock = s;
	return;
}

HANDLE IocpConstructor::getPort()
{
	return this->ComPort;
}

//npc�� �󸶳� ������ ���ΰ�...
void IocpConstructor::AutoNPC(int count)
{
	this->queueLock->LOCK();
	for (int i = 0; i < count; i++)
	{
		//�ʱ��� ������ġ�� �ο����ִ� �۾��� �ʿ�
		NPC temp(i, i, i, i);
		this->manageGame->manageNPC->npc.push_back(temp);
	}
	for (int i = 0; i < count; i++)
	{
		TimerJob temp;
		temp.func = std::bind(&NPC::Move, &(this->manageGame->manageNPC->npc[i]));
		temp.exectime = GetTickCount() + (1000 * i);
		temp.th = i;
		this->jobs.push_back(temp);
	}
	this->queueLock->UNLOCK();
	return;
}



//NPC�� ���� �۾���, �����̵��� ���� 'job'�� ó���� �־���Ѵ�...
void IocpConstructor::JobSchedule()
{
	{
		LockHelper(this->queueLock);

	}
	if (this->jobs.empty()){
		this->queueLock->UNLOCK();
		printf("queue�� ������ϴ�\n");
		return;
	}

	DWORD Minvalue = (1 << 28);
	int index = -1;
	for (int i = 0; i < this->jobs.size(); i++)
	{
		if (this->jobs[i].exectime < Minvalue)
		{
			Minvalue = this->jobs[i].exectime;
			index = i;
		}
	}
	if (index == -1) return;
	TimerJob job = this->jobs[index];
	this->jobs.erase(this->jobs.begin() + index);

	if (job.exectime < GetTickCount())
	{
		int th = job.th;
		auto f = job.func;
		f();

		//���ο� �۾��� ����������..
		//�̵��̳� ���ݿ� ���� ����??
		int action = rand() % 1;
		if (!action)
		{
			//���ǽð� �̵��ϸ� �� ������ ��� Ŭ���̾�Ʈ���� ��������Ѵ�..
			//�� �κ��� Ŭ���� ������ �Űܾ���...Ŭ������ move��!!!
			job.func = std::bind(&NPC::Move, &(this->manageGame->manageNPC->npc[th]));
			job.exectime = GetTickCount() + (500 * th) + 1000;
		}
		else
		{
			//'Attack'�Լ��� �������͸� �Ѱ���� ��� ������ �������� ������ �� �ִ�...��ó�� �ƹ������� ��� ���ݾ���..�ֱٻ��°� �����̾����� �ٽ� ����
			job.func = std::bind(&NPC::Attack, &(this->manageGame->manageNPC->npc[th]), this->cm->mappingClient);
			job.exectime = GetTickCount() + (500 * th) + 1000;
		}
	}
	this->jobs.push_back(job);
	return;
}



void IocpConstructor::ThreadFunction()
{
	SOCKET sock;
	HANDLE hComPort = this->ComPort;
	ClientHandle tempHandle;
	int Ty, DataSize;
	bool hasJob = false;
	char* TempBuffer = new char[BUFSIZE];

	while (1){
		hasJob = GetQueuedCompletionStatus(hComPort, &(tempHandle.bytesTrans), (LPDWORD)&tempHandle.handleinfo, (LPOVERLAPPED*)&(tempHandle.ioinfo), INFINITE);

		printf("%d\n", GetLastError());
		//�ٷ� ����ó������...getlasterror 'INFINITE'��尡 �ƴ� �� ���...
		if (GetLastError() == WAIT_TIMEOUT){
			//������ �ݾ��ش�...
			continue;

		}
		//Ŭ���̾�Ʈ ���
		sock = tempHandle.handleinfo->ClntSock;
		if (hasJob){
			sock = tempHandle.handleinfo->ClntSock;
			USER& User = this->cm->retUser(sock);

			if (tempHandle.ioinfo->rwMode == READ)   
			{
				//�������ῡ ���� �Ϸ� ����
				if (tempHandle.bytesTrans == 0)
				{
					//���������� ����...
					this->cm->removesocket(sock);
					closesocket(sock);
				}

				//user�� ���� ������ �����ϰ�....
				User.setBuffer(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans);
				User.setTotal(tempHandle.bytesTrans);
				User.setCurrent(User.getCurrent() + tempHandle.bytesTrans);
				User.uid = sock;
				User.UserpacketHandle();
				User.clear();

				tempHandle.ReadMode();
				this->RecvMessage(tempHandle);
			}

			//
			else if (tempHandle.ioinfo->rwMode == WRITE)
			{
				//send �κ��� �ٲ���...
				printf("�޽��� ���� �Ϸ�\n");
			
			}
		}

		//��ť ��ó�� -> ��ó���� ��������Ѵ�...
		else{
			printf("�Ϸ������� ���� ���...\n");
			if (GetLastError() == 64){
				this->cm->removesocket(sock);
				closesocket(sock);
				printf("������ ����\n");
			}
			//JobSchedule();
		}
	}
	return;
}