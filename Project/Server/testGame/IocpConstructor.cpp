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
NPCManager* IocpConstructor::nm;
std::vector<TimerJob> IocpConstructor::jobs;
int IocpConstructor::ObjectCount;
Lock* IocpConstructor::ObjectKey;

IocpConstructor::IocpConstructor()
{
	this->manageGame = new GameManager;
	this->cm = new ClientManager;
	this->nm = new NPCManager;
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
void IocpConstructor::AutoNPC(int count)
{


	return;
}


//NPC에 관한 작업과, 유저이동에 관한 'job'을 처리해 주어야한다...
void IocpConstructor::JobSchedule()
{
	
	//'lock'이 여기 있어야되는게 맞나....
	TimerJob job;
	int index = -1;
	{
		LOCKING(this->queueLock);
		//printf("위는 락걸림\n");
		if (this->jobs.empty()){
			//printf("queue가 비었습니다\n");
			return;
		}

		DWORD Minvalue = MAXGETTICK;
		//최소시간이 제일 작을 것을 찾음..
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
		this->jobs.erase(this->jobs.begin() + index);
	}

	if (job.exectime < GetTickCount() && index != -1)
	{
		//함수를 처리해주고...
		int th = job.th;
		auto f = job.func;
		if (f == NULL) return;
		printf("작업처리\n");
		f();
		//최초의 잡을 어디서 해주냐...생각해보자 
		//if (job.current == TimerJob::state::UserMove)


		//새로운 작업을 등록해줘야함..
		//이동이나 공격에 대한 동작??
		//int action = rand() % 1;
		//if (!action)
		//{
		//	//엔피시가 이동하면 이 정보를 모든 클라이언트한테 보내줘야한다..
		//	//이 부분을 클래스 안으로 옮겨야함...클래스내 move로!!!
		//	job.func = std::bind(&NPC::Move, &(this->manageGame->manageNPC->npc[th]));
		//	job.exectime = GetTickCount() + (500 * th) + 1000;
		//}
		//else
		//{
		//	//'Attack'함수에 유저벡터를 넘겨줘야 어느 유저를 공격할지 결정할 수 있다...근처에 아무도없을 경우 공격안함..최근상태가 공격이었으면 다시 공격
		//	job.func = std::bind(&NPC::Attack, &(this->manageGame->manageNPC->npc[th]), this->cm->mappingClient);
		//	job.exectime = GetTickCount() + (500 * th) + 1000;
		//}
	}
	//현재 시간보다 작으면 넣어줌!
	else{
		LOCKING(this->queueLock);
		this->jobs.push_back(job);
	}
		
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
			//소켓번호로 유저아이디를 알 수 있어야 함...
			std::shared_ptr<USER> User = this->cm->retUser((int)this->cm->retUser((SOCKET)sock));
			if (tempHandle.ioinfo->rwMode == READ)
			{
				//접속종료에 대한 완료 통지
				if (tempHandle.bytesTrans == 0)
				{
					//유저정보의 삭제...
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
				
				////////////////////////삭제부분..
				
			}
			//
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