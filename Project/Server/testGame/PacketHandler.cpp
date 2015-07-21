#include "stdafx.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "../protobuffer/AccountPacket.pb.h"
#include "../protobuffer/InGamePacket.pb.h"

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

bool PacketHandler::HandlePacket(Packet& p){
	//패킷에 관한 처리...
	
	int type = p.getType();
	if (this->HandlerTable[type] == nullptr){
		return false;
	}
	this->HandlerTable[type](p);
	return true;
}

void PacketHandler::SendAccountPacket(SOCKET clnt)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size, type;
	size = sizeof(SOCKET);
	type = PKT_ACCOUNT;
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	memcpy(buffer + sizeof(size)+sizeof(type), &clnt, sizeof(SOCKET));
	send(clnt, buffer, size + sizeof(unsigned short)*2, 0);
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
		//temp.set_uid(it->second.uid);  temp.set_hp(it->second.hp); temp.set_x(it->second.x); temp.set_y(it->second.y);
		AccountPacket::S_Account_List::Account *tempAccount = tempList.add_account_member();
		tempAccount->set_uid(it->second.uid); tempAccount->set_hp(it->second.hp); tempAccount->set_x(it->second.x); tempAccount->set_y(it->second.y);
		
	}
	size = tempList.ByteSize();
	printf("%d\n", tempList.ByteSize());
	printf("%d\n", tempList.account_member_size());

	memcpy(buffer, &size, sizeof(size));
	tempList.SerializeToArray(buffer + current, size);
	//이제 보내줘야함..
	for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
	{
		send(it->second.uid, buffer, size + sizeof(unsigned short)*2, 0);
	}
	delete[] buffer;
	return;
}



#define REGIST_HANDLE(PACKET_TYPE, PACKET_HANDLE) \
	void PACKET_HANDLE(Packet& p); \
	HandlerRegistHelper Handler_##PACKET_HANDLE(PACKET_TYPE, PACKET_HANDLE);


REGIST_HANDLE(PKT_C_STOP, C_MOVE_Handler);
void C_MOVE_Handler(Packet& p)
{
	InGamePacket::C_Move MovePacket;


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

