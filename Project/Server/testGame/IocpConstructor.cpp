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
		//클라이언트 통신
		sock = tempHandle.handleinfo->ClntSock;
		if (hasJob){
			printf("완료 통지 \n");
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
					printf("완료 통지 유저나감\n");
				}
				

				//같다는 말은 아직 받은 데이터가 하나도 존재하지 않는다는 의미
				else if (User.getCurrent() == 0)
				{
					//최초의 입장패킷에 대한 처리를 여기서 해줘야함..


					//패킷을 한 번에 받지 못하는 경우까지 생각해줘야함
					unsigned short Len, current = 0;//패킷바디에 관한 사이즈가 저장되어 있음
					memcpy(&Len, tempHandle.ioinfo->wsaBuf.buf, sizeof(Len));
					Len += (sizeof(unsigned short)* 2);
					//받아야 될 전체 길이를 저장하도록 하자
					User.setTotal(Len);
					User.setBuffer(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans);
					User.setCurrent(tempHandle.bytesTrans);
				}

				//아직 다 받지 못한 경우에 대한 예외처리...
				else if (User.getCurrent() != User.getTotal())
				{
					User.setBuffer(tempHandle.ioinfo->wsaBuf.buf, tempHandle.bytesTrans);
					User.setCurrent(User.getCurrent() + tempHandle.bytesTrans);
				}


				if (User.finished())
				{
					//Packet receive;
					////패킷을 분리하고
					//receive.PacketSeperate(User.getBuffer(), User.getTotal());
					////패킷을 처리해주어야함..
					//PacketHandler::GetInstance()->HandlePacket(receive);

					////유저정보에 관한 초기화...
					//User.clear();
				}

				//unsigned short pksize, ty;
				//memcpy(&pksize, tempHandle.ioinfo->wsaBuf.buf, sizeof(pksize));
				//memcpy(&ty, tempHandle.ioinfo->wsaBuf.buf + sizeof(pksize), sizeof(ty));
				//if (ty == C_MOVE){

				//	InGamePacket::C_Move movetemp;
				//	movetemp.ParseFromArray(tempHandle.ioinfo->buf + sizeof(pksize)+sizeof(ty), pksize);
				//	printf("유저의 처음 위치 %d %d\n", User.x, User.y);
				//	printf("이동중!!\n");
				//	if (movetemp.direction() == RIGHT){
				//		User.x += (SPEED*movetemp.duration());
				//	}
				//	else{
				//		User.x -= (SPEED*movetemp.duration());
				//	}
				//	printf("유저의 변경된 위치 %d %d\n", User.x, User.y);

				//	InGamePacket::S_Move movetemp_2;
				//	movetemp_2.set_uid(sock); movetemp_2.set_x(User.x); movetemp_2.set_y(User.y);
				//	pksize = movetemp_2.ByteSize();
				//	ty = S_MOVE;

				//	//IOCP 모든 클라한테 다 보내줘야함...
				//	std::map<SOCKET, USER>::iterator it;
				//	for (it = this->cm->mappingClient.begin(); it != this->cm->mappingClient.end(); it++){
				//		tempHandle.WriteMode();
				//		memcpy(tempHandle.ioinfo->buf, &pksize, sizeof(pksize));
				//		memcpy(tempHandle.ioinfo->buf + sizeof(pksize), &ty, sizeof(ty));
				//		movetemp_2.SerializePartialToArray(tempHandle.ioinfo->buf + sizeof(pksize)+sizeof(ty), pksize);
				//		this->SndMessage(tempHandle, it->second.uid);
				//	}


					////////일반버전
					//memcpy(buffer, &pksize, sizeof(pksize));
					//memcpy(buffer + sizeof(pksize), &ty, sizeof(ty));
					//movetemp_2.SerializePartialToArray(buffer + sizeof(pksize)+sizeof(ty), pksize);
					//send(sock, buffer, pksize + sizeof(pksize)+sizeof(ty), 0);
			//	}
				//데이터의 크기확인
				//패킷의 종류확인
				//유저의 상태에 따라 데이터를 처리여부 결정..
				//미사일이냐 유저정보냐
				//if(미사일) 데미지처리...맵상태..
				//if(유저) 이동이냐 공격이냐


			}
			else if (tempHandle.ioinfo->rwMode == WRITE)
			{
				printf("메시지 전송 완료\n");
				//체력이 '0'인 유저에 대한 처리
				//처리한 정보를 클라이언트에게 돌려줘야함..
				//입은데미지..이런거..?			
			}
		}

		//잡큐 일처리 -> 락처리를 잘해줘야한다...
		else{
			printf("완료통지가 없는 경우\n");
			if (GetLastError() == 64){
				this->cm->removesocket(sock);
				closesocket(sock);
				printf("유저나감\n");
				printf("lasterror : %d\n", GetLastError());
			}
			//JobSchedule();
		}
	}
	return;
}