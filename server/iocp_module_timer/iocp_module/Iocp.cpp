#include "stdafx.h"
#include "Iocp.h"
#include "../character.pb.h"

Iocp::Iocp()
{
	this->queueLock = new Lock();
	this->cm = new ClientManager;
	this->manageGame = new GameManager;
	this->flags = 0;
	this->ComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&this->sysinfo);
	for (int i = 0; i < this->sysinfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, startThread, this, 0, NULL);
	return;
}
Iocp::~Iocp()
{
	delete this->cm;
}
//스레드 생성까지...
void Iocp::makeObject(){



	return;
}
void Iocp::registerOjbect(ClientHandle& client){
	CreateIoCompletionPort((HANDLE)client.ClntSock, this->ComPort, (DWORD)client.handleinfo, 0);
	return;
}

void Iocp::RecvMessage(ClientHandle& client){
	WSARecv(client.handleinfo->ClntSock, &(client.ioInfo->wsaBuf), 1, &(this->recvBytes), &(this->flags), &(client.ioInfo->overlapped), NULL);
	return;
}

void Iocp::SndMessage(ClientHandle& client, SOCKET sock){
	WSASend(sock, &(client.ioInfo->wsaBuf), 1, NULL, 0, &(client.ioInfo->overlapped), NULL);
	return;
}

unsigned WINAPI Iocp::startThread(void *th){
	Iocp *Thread = (Iocp*)th;
	Thread->ThreadFunction();
	return 0;
}

void Iocp::setServerSock(SOCKET s){
	this->ServerSock = s;
	return;
}

HANDLE Iocp::getport(){
	return this->ComPort;
}


void Iocp::initTimer(){

	for (int i = 0; i < 10; i++){
		TimerJob temp;
		temp.func = std::bind(&NPC::Move, &(this->manageGame->manageNPC->_NPC[i]) );
		temp.exectime = GetTickCount() + (1000 * i);
		temp.npc_th = i;
		this->jobs.push(temp);
	}
	return;
}

void Iocp::ThreadFunction(){

	SOCKET sock;
	HANDLE hComPort = (HANDLE)this->ComPort;
	ClientHandle tempHandle;
	int Ty, DataSize;
	bool hasJob = false;

	while (1){
		hasJob = GetQueuedCompletionStatus(hComPort, &(tempHandle.bytesTrans), (LPDWORD)&tempHandle.handleinfo, (LPOVERLAPPED*)&(tempHandle.ioInfo), 10);
		if (hasJob){
			sock = tempHandle.handleinfo->ClntSock;
			int index = this->cm->retIndex(sock);
			ClientData& manage = this->cm->clientdata[index];
			printf("index %d \n", index);


			if (tempHandle.ioInfo->rwMode == READ){
				puts("Message Receive");
				if (tempHandle.bytesTrans == 0){
					this->cm->removesocket(sock);
					closesocket(sock);
					delete tempHandle.handleinfo;
					delete tempHandle.ioInfo;
					continue;
				}
				int total = 0;
				while (total < tempHandle.bytesTrans){
					memcpy(&DataSize, tempHandle.ioInfo->wsaBuf.buf, sizeof(DataSize));
					//헤더의 길이는 계속 더해줘야함..
					total += (DataSize + 8);
					printf("입력받은 데이터 크기:%d\n", DataSize);
					//입력받은 데이터와 크기를 버퍼에 저장
					manage.setLock();
					manage.setTotal(DataSize);
					memcpy(manage.getMessage(0), tempHandle.ioInfo->wsaBuf.buf + sizeof(DataSize), tempHandle.bytesTrans - sizeof(DataSize));
					memcpy(&Ty, tempHandle.ioInfo->wsaBuf.buf + sizeof(DataSize), sizeof(Ty));

					printf("입력받은 타입 %d\n", Ty);
					if (Ty == 0){
						Character::Player temp;
						temp.ParseFromArray(tempHandle.ioInfo->wsaBuf.buf + sizeof(DataSize)+sizeof(Ty), DataSize);
						temp.ParseFromArray(manage.getMessage(sizeof(DataSize)), DataSize - sizeof(DataSize));
						printf("%s %s\n", temp.name().c_str(), temp.chat().c_str());
					}
					else if (Ty == 1){
						Character::NPC temp;
						temp.ParseFromArray(tempHandle.ioInfo->wsaBuf.buf + sizeof(DataSize)+sizeof(Ty), DataSize);
						temp.ParseFromArray(manage.getMessage(sizeof(DataSize)), DataSize - sizeof(DataSize));
						printf("%s %d %d\n", temp.name().c_str(), temp.x(), temp.y());
					}
					else if (Ty == 2){
						Character::Monster temp;
						temp.ParseFromArray(tempHandle.ioInfo->wsaBuf.buf + sizeof(DataSize)+sizeof(Ty), DataSize);

						printf("%s %d\n", temp.name().c_str(), temp.power());
					}
				}
				manage.releaseLock();

				tempHandle.WriteMode();
				this->SndMessage(tempHandle, sock);
				tempHandle.ReadMode();
				this->RecvMessage(tempHandle);
			}
			else if (tempHandle.ioInfo->rwMode == WRITE){
				printf("메시지를 보냄\n");
			}
			//연관없는 메시지일 경우
			else{



			}
		}
		//시간초과가 났을경우 잡큐에 관한 검사를 한다. 
		else{
			this->queueLock->LOCK();
			if (this->jobs.empty()){
				printf("queue가 비었습니다\n");
				continue;
			}
			TimerJob job = this->jobs.front();
			if (job.exectime < GetTickCount()){
				int NPC_th = job.npc_th;
				//printf("몇 번 : %d\n", NPC_th);
				auto f = job.func;
				f();
				job.func = std::bind(&NPC::Move, &(this->manageGame->manageNPC->_NPC[NPC_th]));
				job.exectime = GetTickCount() + (10 * NPC_th);
				this->jobs.push(job);
				this->jobs.pop();
			}
			this->queueLock->UNLOCK();
		}
		
	}
	return;
}