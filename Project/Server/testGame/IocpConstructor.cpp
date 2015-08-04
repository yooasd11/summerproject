#include "stdafx.h"
#include "PacketHandler.h"
#include "IocpConstructor.h"
#include "../protobuffer/AccountPacket.pb.h"
#include "../protobuffer/InGamePacket.pb.h"

//접근하는 방법
//HandlerMap::GetInstance()->HandlePacket(packet);


//'static'으로 선언한 경우 'cpp'에 다시 정의 해주어야 한다.
ObjectManager* IocpConstructor::Object_Manager;
std::vector<TimerJob> IocpConstructor::jobs;
int IocpConstructor::ObjectCount;
Lock* IocpConstructor::ObjectKey;
Lock* IocpConstructor::queueLock;

IocpConstructor::IocpConstructor()
{
	this->Object_Manager = new ObjectManager;
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
	delete this->Object_Manager;
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
//void IocpConstructor::GENERATE_NPC(int count)
//{
//	LOCKING(this->queueLock);
//	for (int i = 0; i < count; i++)
//	{
//		std::shared_ptr<NPC> tempNPC(new NPC(100.0f, 100.0f, 100.0f, 100.0f));  // x , y , vx, vy 
//		IocpConstructor::Object_Manager->REGIST(Object_NPC, 0);
//		TimerJob job;
//		job.exectime = GetTickCount() + NEXT_TICK;
//		job.func = std::bind(&NPC::NPC_INIT, tempNPC);
//		IocpConstructor::jobs.push_back(job);
//	}
//	return;
//}


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
	/*LOCKING(this->queueLock);
	if (this->jobs.empty()) return;
	else jobs.clear();
	return;*/
}



void IocpConstructor::closeSocket(SOCKET sock)
{
	int index = IocpConstructor::Object_Manager->FIND_USER(sock);
	PacketHandler::GetInstance()->S_DISCONNECT_HANDLER(index);
	IocpConstructor::Object_Manager->REMOVE(index);
	return;
}

void IocpConstructor::ThreadFunction()
{
	SOCKET sock;
	HANDLE hComPort = this->ComPort;
	ClientHandle tempHandle;
	Packet ReceivePacket;
	int Ty, DataSize, index;
	bool hasJob = false;

	while (1){
		hasJob = GetQueuedCompletionStatus(hComPort, &(tempHandle.bytesTrans), (LPDWORD)&tempHandle.handleinfo, (LPOVERLAPPED*)&(tempHandle.ioinfo), 30);

		if (hasJob){

			sock = tempHandle.handleinfo->ClntSock;
			index = IocpConstructor::Object_Manager->FIND_USER(sock);


			//if (index == -1) 예외처리하기
			std::shared_ptr<USER> User = std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(index));

			if (tempHandle.ioinfo->rwMode == READ)
			{
				//접속종료에 대한 완료 통지
				if (tempHandle.bytesTrans == 0)
				{
					TimerJob disConnectJob;
					disConnectJob.exectime = GetTickCount();
					disConnectJob.func = std::bind(&IocpConstructor::closeSocket, this, sock);
					LOCKING(this->queueLock)
					{
						this->jobs.push_back(disConnectJob);
					}
					User->ChangeState(USER::state::DISCONNECT);
					continue;
				}

				ReceivePacket.PacketSeperate(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans, sock);

				tempHandle.ReadMode();
				this->RecvMessage(tempHandle);
			}
		}
		//	else if (tempHandle.ioinfo->rwMode == WRITE)
		//	{
		//		if (tempHandle.ioinfo->wsaBuf.len == tempHandle.bytesTrans)
		//		{
		//			delete tempHandle.ioinfo;
		//		}

		//		//비정상으로 인지하고 종료함..
		//		else
		//		{
		//			sock = tempHandle.handleinfo->ClntSock;
		//			index = IocpConstructor::Object_Manager->FIND_USER(sock);
		//			//inf(index == -1) 예외처리하기
		//			std::shared_ptr<USER> User = std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(index));
		//			TimerJob disConnectJob;
		//			disConnectJob.exectime = GetTickCount();
		//			disConnectJob.func = std::bind(&IocpConstructor::closeSocket, this, sock);
		//			LOCKING(this->queueLock)
		//			{
		//				this->jobs.push_back(disConnectJob);
		//			}
		//			User->ChangeState(USER::state::DISCONNECT);
		//		}
		//	}
		//}
		else
		{
			JobSchedule();
		}
	}
	return;
}