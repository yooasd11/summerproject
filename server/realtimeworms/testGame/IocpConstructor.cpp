#include "stdafx.h"
#include "IocpConstructor.h"
#include "../protobuffer/AccountPacket.pb.h"
#include "../protobuffer/InGamePacket.pb.h"


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


void IocpConstructor::JobSchedule()
{
	this->queueLock->LOCK();
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
	this->queueLock->UNLOCK();
	return;
}

void IocpConstructor::ThreadFunction()
{
	SOCKET sock;
	HANDLE hComPort = this->ComPort;
	ClientHandle tempHandle;
	int Ty, DataSize;
	bool hasJob = false;
	char* buffer = new char[500];

	while (1){
		hasJob = GetQueuedCompletionStatus(hComPort, &(tempHandle.bytesTrans), (LPDWORD)&tempHandle.handleinfo, (LPOVERLAPPED*)&(tempHandle.ioinfo), INFINITE);

		//Ŭ���̾�Ʈ ���
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
					printf("��������\n");
				}
				//������ ������Ŷ�� ���� ó���� ���⼭ �������..

				unsigned short pksize, ty;
				memcpy(&pksize, tempHandle.ioinfo->wsaBuf.buf, sizeof(pksize));
				memcpy(&ty, tempHandle.ioinfo->wsaBuf.buf + sizeof(pksize), sizeof(ty));
				if (ty == C_MOVE){

					InGamePacket::C_Move movetemp;
					movetemp.ParseFromArray(tempHandle.ioinfo->buf + sizeof(pksize)+sizeof(ty), pksize);
					printf("������ ó�� ��ġ %d %d\n", User.x, User.y);
					printf("�̵���!!\n");
					if (movetemp.direction() == RIGHT){
						User.x += (SPEED*movetemp.duration());
					}
					else{
						User.x -= (SPEED*movetemp.duration());
					}
					printf("������ ����� ��ġ %d %d\n", User.x, User.y);

					InGamePacket::S_Move movetemp_2;
					movetemp_2.set_uid(sock); movetemp_2.set_x(User.x); movetemp_2.set_y(User.y);
					pksize = movetemp_2.ByteSize();
					ty = S_MOVE;

					//IOCP ��� Ŭ������ �� ���������...
					std::map<SOCKET, USER>::iterator it;
					for (it = this->cm->mappingClient.begin(); it != this->cm->mappingClient.end(); it++){
						tempHandle.WriteMode();
						memcpy(tempHandle.ioinfo->buf, &pksize, sizeof(pksize));
						memcpy(tempHandle.ioinfo->buf + sizeof(pksize), &ty, sizeof(ty));
						movetemp_2.SerializePartialToArray(tempHandle.ioinfo->buf + sizeof(pksize)+sizeof(ty), pksize);
						this->SndMessage(tempHandle, it->second.uid);
					}


					////////�Ϲݹ���
					//memcpy(buffer, &pksize, sizeof(pksize));
					//memcpy(buffer + sizeof(pksize), &ty, sizeof(ty));
					//movetemp_2.SerializePartialToArray(buffer + sizeof(pksize)+sizeof(ty), pksize);
					//send(sock, buffer, pksize + sizeof(pksize)+sizeof(ty), 0);
				}
				//�������� ũ��Ȯ��
				//��Ŷ�� ����Ȯ��
				//������ ���¿� ���� �����͸� ó������ ����..
				//�̻����̳� ����������
				//if(�̻���) ������ó��...�ʻ���..
				//if(����) �̵��̳� �����̳�


			}
			else if (tempHandle.ioinfo->rwMode == WRITE)
			{
				printf("�޽��� ���� �Ϸ�\n");
				//ü���� '0'�� ������ ���� ó��
				//ó���� ������ Ŭ���̾�Ʈ���� ���������..
				//����������..�̷���..?
			
			
			}
		}
		//��ť ��ó�� -> ��ó���� ��������Ѵ�...
		else{
			JobSchedule();
		}
	}
	return;
}