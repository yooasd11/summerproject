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
NPCManager* IocpConstructor::nm;
std::vector<TimerJob> IocpConstructor::jobs;



IocpConstructor::IocpConstructor()
{
	this->manageGame = new GameManager;
	this->cm = new ClientManager;
	this->nm = new NPCManager;
	this->queueLock = new Lock;
	this->UserLock = new Lock;
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
	delete this->UserLock;
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
	

	return;
}


//NPC�� ���� �۾���, �����̵��� ���� 'job'�� ó���� �־���Ѵ�...
void IocpConstructor::JobSchedule()
{
	//'lock'�� ���� �־�ߵǴ°� �³�....
	LOCKING(this->queueLock);
	if (this->jobs.empty()){
		//printf("queue�� ������ϴ�\n");
		return;
	}

	DWORD Minvalue = MAXGETTICK;
	int index = -1;
	//�ּҽð��� ���� ���� ���� ã��..
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
		//�Լ��� ó�����ְ�...
		int th = job.th;
		auto f = job.func;
		f();
		//������ ���� ��� ���ֳ�...�����غ��� 
		//if (job.current == TimerJob::state::UserMove)


		//���ο� �۾��� ����������..
		//�̵��̳� ���ݿ� ���� ����??
		//int action = rand() % 1;
		//if (!action)
		//{
		//	//���ǽð� �̵��ϸ� �� ������ ��� Ŭ���̾�Ʈ���� ��������Ѵ�..
		//	//�� �κ��� Ŭ���� ������ �Űܾ���...Ŭ������ move��!!!
		//	job.func = std::bind(&NPC::Move, &(this->manageGame->manageNPC->npc[th]));
		//	job.exectime = GetTickCount() + (500 * th) + 1000;
		//}
		//else
		//{
		//	//'Attack'�Լ��� �������͸� �Ѱ���� ��� ������ �������� ������ �� �ִ�...��ó�� �ƹ������� ��� ���ݾ���..�ֱٻ��°� �����̾����� �ٽ� ����
		//	job.func = std::bind(&NPC::Attack, &(this->manageGame->manageNPC->npc[th]), this->cm->mappingClient);
		//	job.exectime = GetTickCount() + (500 * th) + 1000;
		//}
	}
	//���� �ð����� ������ �־���!
	else this->jobs.push_back(job);
	return;
}



void IocpConstructor::closeSocket(SOCKET sock)
{
	PacketHandler::GetInstance()->C_DISCONNECT_Handler(sock);
	this->cm->removesocket(sock);
	return;
}

void IocpConstructor::ThreadFunction()
{
	SOCKET sock;
	HANDLE hComPort = this->ComPort;
	ClientHandle tempHandle;
	int Ty, DataSize;
	bool hasJob = false;

	while (1){
		hasJob = GetQueuedCompletionStatus(hComPort, &(tempHandle.bytesTrans), (LPDWORD)&tempHandle.handleinfo, (LPOVERLAPPED*)&(tempHandle.ioinfo), 100);

		if (hasJob){
			sock = tempHandle.handleinfo->ClntSock;
			std::shared_ptr<USER> User = this->cm->retUser(sock);

			if (tempHandle.ioinfo->rwMode == READ)
			{
				//�������ῡ ���� �Ϸ� ����
				if (tempHandle.bytesTrans == 0)
				{
					printf("��������\n");
					//���������� ����...
					TimerJob disConnectJob;
					disConnectJob.exectime = GetTickCount() + 500;
					disConnectJob.func = std::bind(&IocpConstructor::closeSocket, this, sock);
					this->jobs.push_back(disConnectJob);
					User->connect = false;
					continue;
				}
				//user�� ���� ������ �����ϰ�....
				User->setBuffer(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans);
				User->setTotal(tempHandle.bytesTrans);
				User->uid = sock;
				User->UserpacketHandle();

				tempHandle.ReadMode();
				this->RecvMessage(tempHandle);
			}
			//
			else if (tempHandle.ioinfo->rwMode == WRITE)
			{
				//send �κ��� �ٲ���...
			}
		}
		//��ť ��ó�� -> ��ó���� ��������Ѵ�...
		else
		{
			JobSchedule();
		}
	}
	return;
}