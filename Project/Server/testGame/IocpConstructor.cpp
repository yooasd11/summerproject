#include "stdafx.h"
#include "PacketHandler.h"
#include "IocpConstructor.h"
#include "../protobuffer/AccountPacket.pb.h"
#include "../protobuffer/InGamePacket.pb.h"

//접근하는 방법
//HandlerMap::GetInstance()->HandlePacket(packet);


//'static'으로 선언한 경우 'cpp'에 다시 정의 해주어야 한다.
GameManager* IocpConstructor::manageGame;
ClientManager* IocpConstructor::cm;
AIManager* IocpConstructor::nm;
std::vector<TimerJob> IocpConstructor::jobs;
int IocpConstructor::ObjectCount;
Lock* IocpConstructor::ObjectKey;
Lock* IocpConstructor::queueLock;

IocpConstructor::IocpConstructor()
{
	this->manageGame = new GameManager;
	this->cm = new ClientManager;
	this->nm = new AIManager;
	this->queueLock = new Lock;
	this->UserLock = new Lock;
	this->ObjectKey = new Lock;
	this->flags = 0;
	this->ComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	this->ObjectCount = 1;
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
	delete this->ObjectKey;
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

//npc를 얼마나 생성할 것인가...
void IocpConstructor::generateAI(int count)
{
	std::shared_ptr<AI> Instance;
	LOCKING(this->queueLock);
	for (int i = 0; i < count; i++)
	{
		//AI 생성과 등록 동작
		{
			//LOCKING(IocpConstructor::ObjectKey)
			//std::shared_ptr<AI> temp(new AI(IocpConstructor::ObjectCount, 100, rand()%600+20.0f, rand()%250+20.0f, 90.0f, 30.0f));
			std::shared_ptr<AI> temp(new AI(IocpConstructor::ObjectCount, 100, 100.0f, 100.0f, 90.0f, 30.0f));
			Instance = temp;
			Instance->current = Instance->state::alive;
			IocpConstructor::nm->registAI(Instance);
		}
		TimerJob job;
		job.exectime = GetTickCount() + NEXT_TICK;
		job.func = std::bind(&AI::Action, Instance);
		IocpConstructor::jobs.push_back(job);
	}
	return;
}


//NPC에 관한 작업과, 유저이동에 관한 'job'을 처리해 주어야한다...
void IocpConstructor::JobSchedule()
{
	TimerJob job;
	int index = -1;
	{
		LOCKING(this->queueLock);
		if (this->jobs.empty()){
			return;
		}
		DWORD Minvalue = MAXGETTICK;
		for (int i = 0; i < this->jobs.size(); i++)
		{
			if (this->jobs[i].exectime < Minvalue)
			{
				Minvalue = this->jobs[i].exectime;
				index = i;
			}
		}
		if (index == -1) return;
		job = this->jobs[index];
		if (job.exectime > GetTickCount()) return;
		this->jobs.erase(this->jobs.begin() + index);
	}
	auto f = job.func;
	if (f == NULL) return;
	f();
	return;
}



void IocpConstructor::closeSocket(SOCKET sock)
{
	int index = IocpConstructor::cm->retUser((SOCKET)sock);
	PacketHandler::GetInstance()->C_DISCONNECT_Handler(index);
	this->cm->removesocket(index);
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
		hasJob = GetQueuedCompletionStatus(hComPort, &(tempHandle.bytesTrans), (LPDWORD)&tempHandle.handleinfo, (LPOVERLAPPED*)&(tempHandle.ioinfo), 30);

		if (hasJob){
			sock = tempHandle.handleinfo->ClntSock;
			std::shared_ptr<USER> User = this->cm->retUser((int)this->cm->retUser((SOCKET)sock));
			if (tempHandle.ioinfo->rwMode == READ)
			{
				//접속종료에 대한 완료 통지
				if (tempHandle.bytesTrans == 0)
				{
					TimerJob disConnectJob;
					disConnectJob.exectime = GetTickCount() + 500;
					disConnectJob.func = std::bind(&IocpConstructor::closeSocket, this, sock);
					this->jobs.push_back(disConnectJob);
					User->connect = false;
					continue;
				}
				//user에 관한 정보를 저장하고....
				//User->setBuffer(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans);
				//User->setTotal(tempHandle.bytesTrans);
				//User->uid = sock;
				//User->UserpacketHandle();
				User->UserpacketHandle(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans, sock);

				tempHandle.ReadMode();
				this->RecvMessage(tempHandle);				
			}
			else if (tempHandle.ioinfo->rwMode == WRITE)
			{
			
				//send 부분을 바꾸자...
			}
		}
		//잡큐 일처리 -> 락처리를 잘해줘야한다...
		else
		{
			JobSchedule();
		}
	}
	return;
}