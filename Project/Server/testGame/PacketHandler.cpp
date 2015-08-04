#include "stdafx.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "../protobuffer/AccountPacket.pb.h"
#include "../protobuffer/InGamePacket.pb.h"

void BroadCast(char *buffer, int size);

PacketHandler::PacketHandler()
{
	//for (int i = 0; i < 10; i++)
	//	this->HandlerTable[i] = nullptr;
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



////접속했을시 자식의 'uid'를 전달
void PacketHandler::SendAccountPacket(SOCKET clnt, int index)
{
	ClientHandle tempHandle;
	char buffer[BUFSIZE] = { 0, };
	memset(buffer, 0, sizeof(buffer));
	unsigned short size, type;
	size = sizeof(SOCKET);
	type = PKT_ACCOUNT;
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	memcpy(buffer + sizeof(size)+sizeof(type), &index, sizeof(int));

	
	//비동기식으로 데이터를 전송
	tempHandle.ioinfo = new IoData;
	memset(&tempHandle.ioinfo->overlapped, 0, sizeof(OVERLAPPED));
	tempHandle.ioinfo->wsaBuf.len = sizeof(SOCKET)+sizeof(unsigned short)* 2;
	tempHandle.ioinfo->wsaBuf.buf = buffer;
	tempHandle.ioinfo->rwMode = WRITE;
	WSASend(clnt, &(tempHandle.ioinfo->wsaBuf), 1, NULL, 0, &(tempHandle.ioinfo->overlapped), NULL);
	return;
}

void PacketHandler::BroadCastAccountPacket()
{
	char buffer[BUFSIZE] = { 0, };
	memset(buffer, 0, sizeof(buffer));
	unsigned short size=0, type, current = 0;

	AccountPacket::S_Account_List tempList;

	type = PKT_ACCOUNT_LIST;
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	current = (sizeof(unsigned short)*2);
	for (auto it : IocpConstructor::Object_Manager->OBJECT_MAP)
	{
		if (it.second->type == Object_USER || it.second->type == Object_NPC)
		{
			AccountPacket::S_Account_List::Account *tempAccount = tempList.add_account_member();
			tempAccount->set_uid(it.second->ObjectId);
			tempAccount->set_hp(it.second->hp);
			tempAccount->set_x(it.second->x);
			tempAccount->set_y(it.second->y);
		}

	}
	size = tempList.ByteSize();
	memcpy(buffer, &size, sizeof(size));
	tempList.SerializeToArray(buffer + current, size);
	BroadCast(buffer, size + current);
	return;
}

void PacketHandler::BroadCast(char *buffer, int size)
{
	
	for (auto it : IocpConstructor::Object_Manager->OBJECT_MAP)
	{
		if (it.second->type == Object_USER)
		{
			std::shared_ptr<USER> tempUser = std::static_pointer_cast<USER>(it.second);
			ClientHandle tempHandle;
			tempHandle.ioinfo = new IoData;
			memset(&tempHandle.ioinfo->overlapped, 0, sizeof(OVERLAPPED));
			tempHandle.ioinfo->wsaBuf.len = size;
			tempHandle.ioinfo->wsaBuf.buf = buffer;
			tempHandle.ioinfo->rwMode = WRITE;
			if (tempUser->CurrentState != USER::state::DISCONNECT){
				WSASend(tempUser->socket, &(tempHandle.ioinfo->wsaBuf), 1, NULL, 0, &(tempHandle.ioinfo->overlapped), NULL);
			}
		}
	}
	return;
}

void PacketHandler::S_DISCONNECT_HANDLER(int ObjectID)
{
	char buffer[BUFSIZE] = { 0, };
	unsigned short size = 0, type = 0;

	AccountPacket::S_Account_List::Disconnect disconnect;
	type = PKT_S_DISCONNECT;
	disconnect.set_uid(ObjectID);
	size = disconnect.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	disconnect.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	return;
}

#define REGIST_HANDLE(PACKET_TYPE, PACKET_HANDLE) \
	void PACKET_HANDLE(Packet& p); \
	HandlerRegistHelper Handler_##PACKET_HANDLE(PACKET_TYPE, PACKET_HANDLE);


//REGIST_HANDLE(PKT_C_STOP, C_MOVE_Handler);


void PacketHandler::C_COLLISION_HANDLER(Packet& p)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0, current = 0;
	int receiveType;

	InGamePacket::C_Collision ClientCollisionPacket;
	ClientCollisionPacket.ParseFromArray(p.Msg, p.getLength());
	InGamePacket::S_Collision ServerCollisionPacket;


	std::shared_ptr<OBJECT> ob_1 = IocpConstructor::Object_Manager->FIND(ClientCollisionPacket.uid1());
	std::shared_ptr<OBJECT> ob_2 = nullptr;
	if (ClientCollisionPacket.has_uid2() == true){
		ob_2 = IocpConstructor::Object_Manager->FIND(ClientCollisionPacket.uid2());
	}
	

	if (ob_1->type == Object_USER && ob_2 == nullptr)
	{
		std::shared_ptr<USER> User = std::static_pointer_cast<USER>(ob_1);
		User->ChangeState(USER::state::STOP);
		ServerCollisionPacket.set_uid1(User->ObjectId);
		ServerCollisionPacket.set_x(User->x);
		ServerCollisionPacket.set_y(User->y);

	}
	else if (ob_1->type == Object_NPC && ob_2 == nullptr)
	{
		std::shared_ptr<NPC> Npc = std::static_pointer_cast<NPC>(ob_1);

		Npc->NPC_DIRECTION_CHANGE();
		ServerCollisionPacket.set_uid1(Npc->ObjectId);
		ServerCollisionPacket.set_x(Npc->x);
		ServerCollisionPacket.set_y(Npc->y);

	}
	else if (ob_1->type == Object_BULLET && ob_2 == nullptr)
	{
		std::shared_ptr<BULLET> Bullet = std::static_pointer_cast<BULLET>(ob_1);
		ServerCollisionPacket.set_uid1(Bullet->ObjectId);
		ServerCollisionPacket.set_x(Bullet->x);
		ServerCollisionPacket.set_y(Bullet->y);
	}
	else if (ClientCollisionPacket.has_uid2() == true)
	{
		if (ob_2->type == Object_USER)
		{
			std::shared_ptr<BULLET> Bullet = std::static_pointer_cast<BULLET>(ob_1);
			std::shared_ptr<USER> User = std::static_pointer_cast<USER>(ob_2);
			ServerCollisionPacket.set_uid1(Bullet->ObjectId);
			ServerCollisionPacket.set_uid2(User->ObjectId);
			ServerCollisionPacket.set_hp(User->hp);

		}
		else if (ob_2->type == Object_NPC)
		{
			std::shared_ptr<BULLET> Bullet = std::static_pointer_cast<BULLET>(ob_1);
			std::shared_ptr<NPC> Npc = std::static_pointer_cast<NPC>(ob_2);
			ServerCollisionPacket.set_uid1(Bullet->ObjectId);
			ServerCollisionPacket.set_uid2(Npc->ObjectId);
			ServerCollisionPacket.set_hp(Npc->hp);

		}
		else if (ob_2->type == Object_BULLET)
		{
			std::shared_ptr<BULLET> Bullet = std::static_pointer_cast<BULLET>(ob_1);
			std::shared_ptr<BULLET> Bullet_2 = std::static_pointer_cast<BULLET>(ob_2);
			ServerCollisionPacket.set_uid1(Bullet->ObjectId);
			ServerCollisionPacket.set_uid2(Bullet_2->ObjectId);
		}
	}

	size = ServerCollisionPacket.ByteSize();
	type = PKT_S_COLLISION;
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	return;
}

void PacketHandler::S_COLLISION_HANDLER(std::shared_ptr<OBJECT> ob_1, std::shared_ptr<OBJECT> ob_2)
{
	char buffer[BUFSIZE] = { 0, };
	unsigned short size = 0, type = 0;

	InGamePacket::S_Collision ServerCollisionPacket;
	type = PKT_S_COLLISION;

	//총알 - 맵, 유저 - 맵, 엔피씨 - 맵
	if (ob_2 == NULL)
	{
		ServerCollisionPacket.set_uid1(ob_1->ObjectId);
		ServerCollisionPacket.set_x(ob_1->x);
		ServerCollisionPacket.set_y(ob_1->y);
	}
	//총알 - 유저, 총알 - 엔피씨
	else if (ob_1->type == Object_BULLET && (ob_2->type == Object_USER || ob_2->type == Object_NPC))
	{
		ServerCollisionPacket.set_uid1(ob_1->ObjectId);
		ServerCollisionPacket.set_uid2(ob_2->ObjectId);
		ServerCollisionPacket.set_hp(ob_2->hp);
		ServerCollisionPacket.set_x(ob_2->x);
		ServerCollisionPacket.set_y(ob_2->y);
	}
	//총알 - 총알
	else if (ob_1->type == Object_BULLET && ob_2->type == Object_BULLET)
	{
		ServerCollisionPacket.set_uid1(ob_1->ObjectId);
		ServerCollisionPacket.set_uid2(ob_2->ObjectId);
	}

	size = ServerCollisionPacket.ByteSize();
	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	ServerCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	return;
}

void PacketHandler::C_SHOOT_HANDLER(Packet& p)
{
	char buffer[BUFSIZE] = { 0, };
	unsigned short size = 0, type = 0;
	int index;
	InGamePacket::C_Shoot ClientBullet;
	ClientBullet.ParseFromArray(p.Msg, p.getLength());


	std::shared_ptr<BULLET> Bullet;
	std::shared_ptr<USER> tempUser;
	{
		index = IocpConstructor::Object_Manager->FIND_USER(p.socket);
		tempUser = std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(index));
		std::shared_ptr<BULLET> tempBullet(new BULLET(tempUser->x, tempUser->y, ClientBullet.unit_vx(), ClientBullet.unit_vy(), index));
		Bullet = tempBullet;
		IocpConstructor::Object_Manager->REGIST_BULLET(Bullet);
	}
	//총 움직임에 대한 'JOB'처리
	TimerJob job;
	job.exectime = GetTickCount() + 30;
	job.func = std::bind(&BULLET::BULLET_MOVE, Bullet);  //만약에 안되면 여기 의심
	{
		LOCKING(IocpConstructor::queueLock);
		IocpConstructor::jobs.push_back(job);
	}

	Bullet->vx = Bullet->vx + Bullet->ax * 0.03f;
	Bullet->vy = Bullet->vy + Bullet->ay * 0.03f;

	Bullet->x += (Bullet->vx*0.03f);
	Bullet->y += (Bullet->vy*0.03f);
	

	//AccountPacket::S_Account_List::Account *tempAccount = tempList.add_account_member();


	type = PKT_S_SHOOT;
	InGamePacket::S_Shoot ServerShootPacket;
	ServerShootPacket.set_uid(index);
	ServerShootPacket.set_damage(Bullet->damage);
	ServerShootPacket.set_bullet_uid(Bullet->ObjectId);
	ServerShootPacket.set_x(Bullet->x);
	ServerShootPacket.set_y(Bullet->y);
	ServerShootPacket.set_vx(Bullet->vx);
	ServerShootPacket.set_vy(Bullet->vy);

	InGamePacket::S_Acceleration *temp = ServerShootPacket.add_acceleration_list();
	temp->set_ax(Bullet->ax);
	temp->set_ay(Bullet->ay);

	size = ServerShootPacket.ByteSize();
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerShootPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	return;
}

void PacketHandler::S_SHOOT_HANDLER(std::shared_ptr<OBJECT> ob)
{
	char buffer[BUFSIZE] = { 0, };
	unsigned short size = 0, type = 0;

	InGamePacket::S_Shoot ServerShootPacket;
	std::shared_ptr<BULLET> Bullet = std::static_pointer_cast<BULLET>(ob);

	type = PKT_S_SHOOT;

	ServerShootPacket.set_uid(Bullet->shooter);
	ServerShootPacket.set_damage(Bullet->damage);
	ServerShootPacket.set_bullet_uid(Bullet->ObjectId);
	ServerShootPacket.set_x(Bullet->x);
	ServerShootPacket.set_y(Bullet->y);
	ServerShootPacket.set_vx(Bullet->vx);
	ServerShootPacket.set_vy(Bullet->vy);

	InGamePacket::S_Acceleration *temp = ServerShootPacket.add_acceleration_list();
	temp->set_ax(Bullet->ax);
	temp->set_ay(Bullet->ay);

	size = ServerShootPacket.ByteSize();
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerShootPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	return;

}

void PacketHandler::S_STOP_HANDLER(std::shared_ptr<OBJECT> ob)
{
	char buffer[BUFSIZE] = { 0, };
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Stop ServerStopPacket;
	ServerStopPacket.set_uid(ob->ObjectId);
	ServerStopPacket.set_x(ob->x);
	ServerStopPacket.set_y(ob->y);

	type = PKT_S_STOP;
	size = ServerStopPacket.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	ServerStopPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	return;

}

//stop 패킷 받았을경우 처리방법, 생각해보자..-> "USER"한테 오는 경우만 신경써주면 된다
void PacketHandler::C_STOP_HANDLER(Packet& p)
{
	char buffer[BUFSIZE] = { 0, };
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0, current = 0;
	int receiveType;

	InGamePacket::C_Stop StopPacket;
	StopPacket.ParseFromArray(p.Msg, p.getLength());
	InGamePacket::S_Stop ServerStopPacket;


	std::shared_ptr<OBJECT> tempObject = IocpConstructor::Object_Manager->FIND(StopPacket.uid());
	if (tempObject->type == Object_USER)
	{
		std::shared_ptr<USER> User = std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(StopPacket.uid()));
		User->ChangeState(USER::state::STOP);
		ServerStopPacket.set_uid(StopPacket.uid());
		ServerStopPacket.set_x(StopPacket.x());
		ServerStopPacket.set_y(StopPacket.y());
	}
	/*else if (tempObject->type == Object_NPC)
	{
		std::shared_ptr<NPC> Npc = std::static_pointer_cast<NPC>(IocpConstructor::Object_Manager->FIND(StopPacket.uid()));
		Npc->ChangeDirection();
		ServerStopPacket.set_uid(StopPacket.uid());
		ServerStopPacket.set_x(Npc->x);
		ServerStopPacket.set_y(Npc->y);

	}*/
	else if (tempObject->type == Object_BULLET)
	{
		ServerStopPacket.set_uid(StopPacket.uid());
		ServerStopPacket.set_x(StopPacket.x());
		ServerStopPacket.set_y(StopPacket.y());
	}

	size = ServerStopPacket.ByteSize();
	type = PKT_S_STOP;
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerStopPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	return;
}

void PacketHandler::S_MOVE_HANDLER(std::shared_ptr<OBJECT> ob)
{
	char buffer[BUFSIZE] = { 0, };
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	InGamePacket::S_Move ServerMovePacket;


	ServerMovePacket.set_uid(ob->ObjectId);
	ServerMovePacket.set_x(ob->x);
	ServerMovePacket.set_y(ob->y);
	ServerMovePacket.set_vx(ob->vx);
	ServerMovePacket.set_vy(ob->vy);

	InGamePacket::S_Acceleration *temp = ServerMovePacket.add_acceleration_list();
	temp->set_ax(0.0f);
	temp->set_ay(0.0f);

	size = ServerMovePacket.ByteSize();
	type = PKT_S_MOVE;

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerMovePacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	return;
}

void PacketHandler::C_MOVE_HANDLER(Packet& p)
{
	char buffer[BUFSIZE] = { 0, };
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	//패킷을 받아서 유저의 상태가 이동상태로 변한다...이정보를 모든 클라이언트에게 전달해야함..
	InGamePacket::C_Move MovePacket;
	MovePacket.ParseFromArray(p.Msg, p.getLength());

	std::shared_ptr<USER> user = std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(MovePacket.uid()));
	if (user == NULL || user->CurrentState == USER::state::DEAD || user->CurrentState == USER::state::DISCONNECT) return;

	
	user->ChangeState(USER::state::ALIVE);
	user->SetVelocity(MovePacket.unit_vx(), MovePacket.unit_vy());
	type = PKT_S_MOVE;

	TimerJob job;
	job.current = job.state::UserMove;
	job.exectime = GetTickCount() + 30;
	job.func = std::bind(&USER::USER_MOVE, user);
	{
		LOCKING(IocpConstructor::queueLock);
		IocpConstructor::jobs.push_back(job);
	}
	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(user->ObjectId);
	ServerMovePacket.set_vx(user->vx);
	ServerMovePacket.set_vy(user->vy);
	ServerMovePacket.set_x(user->x);
	ServerMovePacket.set_y(user->y);

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerMovePacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	return;
}

bool PacketHandler::HandlePacket(Packet& p){

	if (p.getType() == PKT_C_MOVE){
		C_MOVE_HANDLER(p);
	}
	else if (p.getType() == PKT_C_STOP){
		C_STOP_HANDLER(p);
	}
	else if (p.getType() == PKT_C_SHOOT){
		C_SHOOT_HANDLER(p);
	}
	else if (p.getType() == PKT_C_COLLISION){
		C_COLLISION_HANDLER(p);
	}
	else
	{
		return false;
	}
	return true;
}
