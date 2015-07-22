#include "stdafx.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "../protobuffer/AccountPacket.pb.h"
#include "../protobuffer/InGamePacket.pb.h"

void BroadCast(char *buffer, int size);

PacketHandler::PacketHandler()
{
	for (int i = 0; i < 10; i++)
		this->HandlerTable[i] = nullptr;
}

PacketHandler::~PacketHandler()
{


}

void PacketHandler::RegistHandler(PACKET_HANDLER_TYPE eType, HandlerFunc func)
{
	this->HandlerTable[eType] = func;
}

void PacketHandler::UnregistHandler(int type)
{
	this->HandlerTable[type] = nullptr;
}



//접속했을시 자식의 'uid'를 전달
void PacketHandler::SendAccountPacket(SOCKET clnt)
{
	ClientHandle tempHandle;
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size, type;
	size = sizeof(SOCKET);
	type = PKT_ACCOUNT;
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	memcpy(buffer + sizeof(size)+sizeof(type), &clnt, sizeof(SOCKET));

	
	//비동기식으로 데이터를 전송
	
	tempHandle.ioinfo = new IoData;
	memset(&tempHandle.ioinfo->overlapped, 0, sizeof(OVERLAPPED));
	tempHandle.ioinfo->wsaBuf.len = sizeof(SOCKET)+sizeof(unsigned short)* 2;
	tempHandle.ioinfo->wsaBuf.buf = buffer;
	tempHandle.ioinfo->rwMode = WRITE;
	WSASend(clnt, &(tempHandle.ioinfo->wsaBuf), 1, NULL, 0, &(tempHandle.ioinfo->overlapped), NULL);
	delete[] buffer;
	return;
}

void PacketHandler::BroadCastAccountPacket()
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size=0, type, current = 0;

	AccountPacket::S_Account_List tempList;

	type = PKT_ACCOUNT_LIST;
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	current = (sizeof(unsigned short)*2);

	std::map<SOCKET, USER>::iterator it;

	//패킷을 만드는 과정...데이터를 읽고 있는데 변환되면 안된다. 'LOCK'이 필요한가
	for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
	{
		AccountPacket::S_Account_List::Account *tempAccount = tempList.add_account_member();
		tempAccount->set_uid(it->second.uid); tempAccount->set_hp(it->second.hp); tempAccount->set_x(it->second.x); tempAccount->set_y(it->second.y);
	}
	size = tempList.ByteSize();

	memcpy(buffer, &size, sizeof(size));
	tempList.SerializeToArray(buffer + current, size);
	BroadCast(buffer, size + current);

	delete[] buffer;
	return;
}


#define REGIST_HANDLE(PACKET_TYPE, PACKET_HANDLE) \
	void PACKET_HANDLE(Packet& p); \
	HandlerRegistHelper Handler_##PACKET_HANDLE(PACKET_TYPE, PACKET_HANDLE);


//REGIST_HANDLE(PKT_C_STOP, C_MOVE_Handler);


void PacketHandler::C_MOVE_Handler(Packet& p)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	//패킷을 받아서 유저의 상태가 이동상태로 변한다...이정보를 모든 클라이언트에게 전달해야함..
	InGamePacket::C_Move MovePacket;
	MovePacket.ParseFromArray(p.Msg, p.getLength());
	USER &user = IocpConstructor::cm->retUser(MovePacket.uid());

	//유저정보 수정...
	user.state = MOVE;
	user.velocity = MovePacket.velocity();
	type = PKT_S_MOVE;

	
	//최초의 작업을 잡큐에 넣어줌..
	TimerJob job;
	job.current = job.state::UserMove;
	job.exectime = GetTickCount() + 100;
	job.func = std::bind(&USER::UserMove, user);
	IocpConstructor::jobs.push_back(job);

	
	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(MovePacket.uid()); ServerMovePacket.set_velocity(MovePacket.velocity());
	ServerMovePacket.set_x(user.x); ServerMovePacket.set_y(user.y);
	size = ServerMovePacket.ByteSize();	

	//버퍼를 만들고, 전달
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerMovePacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	printf("%d %f %f %f\n", ServerMovePacket.uid(), ServerMovePacket.x(), ServerMovePacket.y(), ServerMovePacket.velocity());
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	
	//잡큐에 넣어줘서 계속해서 이동체크를 해주어야 한다....이게 상태바꾸는거...

	delete[] buffer;
	return;
}

void PacketHandler::C_MOVE_Handler(USER user)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(user.uid); ServerMovePacket.set_x(user.x); ServerMovePacket.set_y(user.y); ServerMovePacket.set_velocity(user.velocity);
	size = ServerMovePacket.ByteSize();

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerMovePacket.SerializePartialToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	delete[] buffer;
	return;
}

void PacketHandler::C_STOP_handler(Packet& p)
{
	printf("stop call \n");
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0, current = 0;

	InGamePacket::C_Stop StopPacket;
	StopPacket.ParseFromArray(p.Msg, p.getLength());
	USER &user = IocpConstructor::cm->retUser(StopPacket.uid());
	user.state = WAIT;

	type = PKT_S_STOP;

	InGamePacket::S_Stop ServerStopPacket;
	ServerStopPacket.set_uid(StopPacket.uid()); ServerStopPacket.set_x(user.x); ServerStopPacket.set_y(user.y);
	size = ServerStopPacket.ByteSize();

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerStopPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	
	delete[] buffer;
	return;
}


void PacketHandler::BroadCast(char *buffer, int size)
{
	ClientHandle tempHandle;
	std::map<SOCKET, USER>::iterator it;

	tempHandle.ioinfo = new IoData;
	memset(&tempHandle.ioinfo->overlapped, 0, sizeof(OVERLAPPED));
	tempHandle.ioinfo->wsaBuf.len = size;
	tempHandle.ioinfo->wsaBuf.buf = buffer;
	tempHandle.ioinfo->rwMode = WRITE;
	for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
	{
		WSASend(it->second.uid, &(tempHandle.ioinfo->wsaBuf), 1, NULL, 0, &(tempHandle.ioinfo->overlapped), NULL);
	}
	return;
}

bool PacketHandler::HandlePacket(Packet& p){

	if (p.getType() == PKT_C_MOVE){
		C_MOVE_Handler(p);
	}
	else if (p.getType() == PKT_C_STOP){
		C_STOP_handler(p);
	}


	return true;

	//printf("여기까지 호출??\n");
	//int type = p.getType();
	//if (this->HandlerTable[type] == nullptr){
	//	return false;
	//}
	//this->HandlerTable[type](p);
	//return true;
}
