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
	tempHandle.WriteMode();
	tempHandle.ioinfo->wsaBuf.len = sizeof(SOCKET)+sizeof(unsigned short)* 2;
	tempHandle.ioinfo->wsaBuf.buf = buffer;
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


REGIST_HANDLE(PKT_C_STOP, C_MOVE_Handler);
void C_MOVE_Handler(Packet& p)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	//패킷을 받아서 유저의 상태가 이동상태로 변한다...이정보를 모든 클라이언트에게 전달해야함..
	InGamePacket::C_Move MovePacket;
	MovePacket.ParseFromArray(p.Msg, p.getLength());
	USER &user = IocpConstructor::cm->retUser(MovePacket.uid());
	user.state = MOVE;

	type = PKT_S_MOVE;
	
	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(MovePacket.uid()); ServerMovePacket.set_velocity(MovePacket.velocity());
	ServerMovePacket.set_x(user.x); ServerMovePacket.set_y(user.y);
	size = ServerMovePacket.ByteSize();

	//버퍼를 만들고, 전달
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerMovePacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	//std::map<SOCKET, USER>::iterator it;
	//for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++){
	//	//비동기로 바꿔줘야함
	//	send(it->second.uid, buffer, size + sizeof(unsigned short)* 2, 0);
	//}
	//잡큐에 넣어줘서 계속해서 이동체크를 해주어야 한다....이게 상태바꾸는거...

	delete[] buffer;
	return;
}
//
//REGIST_HANDLE(P_CMOVE, CMOVE_Handler);
//void CMOVE_Handler(Packet& p)
//{
//
//
//
//
//}

void BroadCast(char *buffer, int size)
{
	ClientHandle tempHandle;
	std::map<SOCKET, USER>::iterator it;

	tempHandle.WriteMode();
	tempHandle.ioinfo->wsaBuf.len = size;
	tempHandle.ioinfo->wsaBuf.buf = buffer;
	for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
	{
		WSASend(it->second.uid, &(tempHandle.ioinfo->wsaBuf), 1, NULL, 0, &(tempHandle.ioinfo->overlapped), NULL);
	}
	return;
}

bool PacketHandler::HandlePacket(Packet& p){
	//패킷에 관한 처리...핸들러로 하면 안될거같음

	if (p.getType() == PKT_C_MOVE){
		C_MOVE_Handler(p);
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
