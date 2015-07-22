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

//npc를 얼마나 생성할 것인가...
void IocpConstructor::AutoNPC(int count)
{
	this->queueLock->LOCK();
	for (int i = 0; i < count; i++)
	{
		//초기의 랜덤위치를 부여해주는 작업이 필요
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



//NPC에 관한 작업과, 유저이동에 관한 'job'을 처리해 주어야한다...
void IocpConstructor::JobSchedule()
{
	{
		LockHelper(this->queueLock);

	}
	if (this->jobs.empty()){
		this->queueLock->UNLOCK();
		printf("queue가 비었습니다\n");
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

		//새로운 작업을 등록해줘야함..
		//이동이나 공격에 대한 동작??
		int action = rand() % 1;
		if (!action)
		{
			//엔피시가 이동하면 이 정보를 모든 클라이언트한테 보내줘야한다..
			//이 부분을 클래스 안으로 옮겨야함...클래스내 move로!!!
			job.func = std::bind(&NPC::Move, &(this->manageGame->manageNPC->npc[th]));
			job.exectime = GetTickCount() + (500 * th) + 1000;
		}
		else
		{
			//'Attack'함수에 유저벡터를 넘겨줘야 어느 유저를 공격할지 결정할 수 있다...근처에 아무도없을 경우 공격안함..최근상태가 공격이었으면 다시 공격
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
		//바로 에러처리해줌...getlasterror 'INFINITE'모드가 아닐 때 사용...
		if (GetLastError() == WAIT_TIMEOUT){
			//소켓을 닫아준다...
			continue;

		}
		//클라이언트 통신
		sock = tempHandle.handleinfo->ClntSock;
		if (hasJob){
			sock = tempHandle.handleinfo->ClntSock;
			USER& User = this->cm->retUser(sock);

			if (tempHandle.ioinfo->rwMode == READ)   
			{
				//접속종료에 대한 완료 통지
				if (tempHandle.bytesTrans == 0)
				{
					//유저정보의 삭제...
					this->cm->removesocket(sock);
					closesocket(sock);
				}

				//user에 관한 정보를 저장하고....
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
				//send 부분을 바꾸자...
				printf("메시지 전송 완료\n");
			
			}
		}

		//잡큐 일처리 -> 락처리를 잘해줘야한다...
		else{
			printf("완료통지가 없는 경우...\n");
			if (GetLastError() == 64){
				this->cm->removesocket(sock);
				closesocket(sock);
				printf("비정상 종료\n");
			}
			//JobSchedule();
		}
	}
	return;
}