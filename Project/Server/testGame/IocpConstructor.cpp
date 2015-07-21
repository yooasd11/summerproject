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
		//Ŭ���̾�Ʈ ���
		sock = tempHandle.handleinfo->ClntSock;
		if (hasJob){
			printf("�Ϸ� ���� \n");
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
					printf("�Ϸ� ���� ��������\n");
				}
				

				//���ٴ� ���� ���� ���� �����Ͱ� �ϳ��� �������� �ʴ´ٴ� �ǹ�
				else if (User.getCurrent() == 0)
				{
					//������ ������Ŷ�� ���� ó���� ���⼭ �������..


					//��Ŷ�� �� ���� ���� ���ϴ� ������ �����������
					unsigned short Len, current = 0;//��Ŷ�ٵ� ���� ����� ����Ǿ� ����
					memcpy(&Len, tempHandle.ioinfo->wsaBuf.buf, sizeof(Len));
					Len += (sizeof(unsigned short)* 2);
					//�޾ƾ� �� ��ü ���̸� �����ϵ��� ����
					User.setTotal(Len);
					User.setBuffer(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans);
					User.setCurrent(tempHandle.bytesTrans);
				}

				//���� �� ���� ���� ��쿡 ���� ����ó��...
				else if (User.getCurrent() != User.getTotal())
				{
					User.setBuffer(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans);
					User.setCurrent(User.getCurrent() + tempHandle.bytesTrans);
				}


				if (User.finished())
				{
					//Packet receive;
					////��Ŷ�� �и��ϰ�
					//receive.PacketSeperate(User.getBuffer(), User.getTotal());
					////��Ŷ�� ó�����־����..
					//PacketHandler::GetInstance()->HandlePacket(receive);

					////���������� ���� �ʱ�ȭ...
					//User.clear();
				}

				//unsigned short pksize, ty;
				//memcpy(&pksize, tempHandle.ioinfo->wsaBuf.buf, sizeof(pksize));
				//memcpy(&ty, tempHandle.ioinfo->wsaBuf.buf + sizeof(pksize), sizeof(ty));
				//if (ty == C_MOVE){

				//	InGamePacket::C_Move movetemp;
				//	movetemp.ParseFromArray(tempHandle.ioinfo->buf + sizeof(pksize)+sizeof(ty), pksize);
				//	printf("������ ó�� ��ġ %d %d\n", User.x, User.y);
				//	printf("�̵���!!\n");
				//	if (movetemp.direction() == RIGHT){
				//		User.x += (SPEED*movetemp.duration());
				//	}
				//	else{
				//		User.x -= (SPEED*movetemp.duration());
				//	}
				//	printf("������ ����� ��ġ %d %d\n", User.x, User.y);

				//	InGamePacket::S_Move movetemp_2;
				//	movetemp_2.set_uid(sock); movetemp_2.set_x(User.x); movetemp_2.set_y(User.y);
				//	pksize = movetemp_2.ByteSize();
				//	ty = S_MOVE;

				//	//IOCP ��� Ŭ������ �� ���������...
				//	std::map<SOCKET, USER>::iterator it;
				//	for (it = this->cm->mappingClient.begin(); it != this->cm->mappingClient.end(); it++){
				//		tempHandle.WriteMode();
				//		memcpy(tempHandle.ioinfo->buf, &pksize, sizeof(pksize));
				//		memcpy(tempHandle.ioinfo->buf + sizeof(pksize), &ty, sizeof(ty));
				//		movetemp_2.SerializePartialToArray(tempHandle.ioinfo->buf + sizeof(pksize)+sizeof(ty), pksize);
				//		this->SndMessage(tempHandle, it->second.uid);
				//	}


					////////�Ϲݹ���
					//memcpy(buffer, &pksize, sizeof(pksize));
					//memcpy(buffer + sizeof(pksize), &ty, sizeof(ty));
					//movetemp_2.SerializePartialToArray(buffer + sizeof(pksize)+sizeof(ty), pksize);
					//send(sock, buffer, pksize + sizeof(pksize)+sizeof(ty), 0);
			//	}
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
			printf("�Ϸ������� ���� ���\n");
			if (GetLastError() == 64){
				this->cm->removesocket(sock);
				closesocket(sock);
				printf("��������\n");
				printf("lasterror : %d\n", GetLastError());
			}
			//JobSchedule();
		}
	}
	return;
}