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



//접속했을시 자식의 'uid'를 전달
void PacketHandler::SendAccountPacket(SOCKET clnt, int index)
{
	ClientHandle tempHandle;
	char* buffer = new char[BUFSIZE];
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

	std::map<int, std::shared_ptr<USER>>::iterator it;
	for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
	{
		AccountPacket::S_Account_List::Account *tempAccount = tempList.add_account_member();
		tempAccount->set_uid(it->second->objectID); tempAccount->set_hp(it->second->hp); tempAccount->set_x(it->second->x); tempAccount->set_y(it->second->y);
	}
	std::map<int, std::shared_ptr<AI>>::iterator it_2;
	for (it_2 = IocpConstructor::nm->mappingAI.begin(); it_2 != IocpConstructor::nm->mappingAI.end(); it_2++)
	{
		AccountPacket::S_Account_List::Account *tempAccount = tempList.add_account_member();
		tempAccount->set_uid(it_2->second->nid); tempAccount->set_hp(it_2->second->hp); tempAccount->set_x(it_2->second->x); tempAccount->set_y(it_2->second->y);
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


	//유저를 불러와서 만약 연결이 끊어진 상태라면 패킷을 전송하지 않고 종료
	std::shared_ptr<USER> user = IocpConstructor::cm->retUser((int)MovePacket.uid());
	if (user == NULL) return;
	if (!user->connect) return;

	//user->crt = USER::state::MOVING;
	user->ChangeState(USER::state::MOVING);
	user->direction = MovePacket.direction();
	type = PKT_S_MOVE;

	TimerJob job;
	job.current = job.state::UserMove;
	job.exectime = GetTickCount() + 30;
	job.func = std::bind(&USER::UserMove, user);
	IocpConstructor::jobs.push_back(job);

	
	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(MovePacket.uid()); ServerMovePacket.set_velocity(user->velocity);
	ServerMovePacket.set_x(user->x); ServerMovePacket.set_y(user->y); ServerMovePacket.set_direction(MovePacket.direction());
	size = ServerMovePacket.ByteSize();	


	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerMovePacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	delete[] buffer;
	return;
}

void PacketHandler::C_MOVE_Handler(std::shared_ptr<USER> user)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(user->objectID); ServerMovePacket.set_x(user->x); ServerMovePacket.set_y(user->y); ServerMovePacket.set_velocity(user->velocity);
	ServerMovePacket.set_direction(user->direction);
	size = ServerMovePacket.ByteSize();
	type = PKT_S_MOVE;

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerMovePacket.SerializePartialToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	delete[] buffer;
	return;
}

//유저로 부터 받는 'stop'로직 처리
void PacketHandler::C_STOP_handler(Packet& p)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0, current = 0;
	int receiveType;

	InGamePacket::C_Stop StopPacket;
	StopPacket.ParseFromArray(p.Msg, p.getLength());
	InGamePacket::S_Stop ServerStopPacket;

	if (IocpConstructor::cm->mappingClient.count(StopPacket.uid()) != 0)
	{
		std::shared_ptr<USER> user = IocpConstructor::cm->retUser((int)StopPacket.uid());
		if (user == NULL) return;
		//user->crt = USER::state::WAITING;
		user->ChangeState(USER::state::WAITING);
		ServerStopPacket.set_uid(StopPacket.uid()); ServerStopPacket.set_x(user->x); ServerStopPacket.set_y(user->y);
		size = ServerStopPacket.ByteSize();
	}
	else if (IocpConstructor::nm->mappingAI.count(StopPacket.uid()) != 0)
	{
		std::shared_ptr<AI> ai = IocpConstructor::nm->retAI((int)StopPacket.uid());
		if (ai == NULL) return;
		//ai->current = AI::state::Waiting;


		//ai->ChageState(AI::state::Waiting);
		ai->ChangeDirection();
		ServerStopPacket.set_uid(StopPacket.uid()); ServerStopPacket.set_x(ai->x); ServerStopPacket.set_y(ai->y);
		size = ServerStopPacket.ByteSize();
	}
	else if (IocpConstructor::manageGame->mappingBullet.count(StopPacket.uid())!=0)
	{
		std::shared_ptr<bullet> shoot = IocpConstructor::manageGame->retBullet(StopPacket.uid());
		if (shoot == NULL) return;
		IocpConstructor::manageGame->removeBullet(StopPacket.uid());
		//shoot->working = false;
		shoot->ChangeState(false);
		ServerStopPacket.set_uid(shoot->uid);  ServerStopPacket.set_x(shoot->x); ServerStopPacket.set_y(shoot->y);
		size = ServerStopPacket.ByteSize();
	}
	type = PKT_S_STOP;
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerStopPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	
	delete[] buffer;
	return;
}

void PacketHandler::C_COLLISION_Handler(Packet& p)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0, current = 0;
	int receiveType;

	InGamePacket::C_Collision ClientCollisionPacket;
	ClientCollisionPacket.ParseFromArray(p.Msg, p.getLength());
	InGamePacket::S_Collision ServerCollisionPacket;

	//플레이어가 벽에 부딪친 경우
	if (IocpConstructor::cm->mappingClient.count(ClientCollisionPacket.uid1())!=0)
	{
		std::shared_ptr<USER> user = IocpConstructor::cm->retUser((int)ClientCollisionPacket.uid1());
		if (user == NULL) return;
		//user->crt = USER::state::WAITING;
		user->ChangeState(USER::state::WAITING);
		ServerCollisionPacket.set_uid1(ClientCollisionPacket.uid1()); ServerCollisionPacket.set_x(user->x); ServerCollisionPacket.set_y(user->y);
		size = ServerCollisionPacket.ByteSize();
	}
	//NPC가 부딪친 경우
	else if (IocpConstructor::nm->mappingAI.count(ClientCollisionPacket.uid1()) != 0)
	{
		std::shared_ptr<AI> ai = IocpConstructor::nm->retAI((int)ClientCollisionPacket.uid1());
		if (ai == NULL) return;
		//ai->current = AI::state::Waiting;
		//ai->ChageState(AI::state::Waiting);
		ai->ChangeDirection();
		ServerCollisionPacket.set_uid1(ClientCollisionPacket.uid1()); ServerCollisionPacket.set_x(ai->x); ServerCollisionPacket.set_y(ai->y);
		size = ServerCollisionPacket.ByteSize();
	}
	//총알 - 유저, 총알 - 총알
	else if (ClientCollisionPacket.has_uid2() == true)
	{
		//총알 - 유저
		if (IocpConstructor::cm->mappingClient.count(ClientCollisionPacket.uid2()) != 0){
			std::shared_ptr<bullet> shoot = IocpConstructor::manageGame->retBullet(ClientCollisionPacket.uid1());
			std::shared_ptr<USER> user = IocpConstructor::cm->retUser((int)ClientCollisionPacket.uid2());
			//if (shoot == NULL || user == NULL) return;
			//user->hp -= shoot->damage;
			//if (user->hp <= 0){
				//user->crt = USER::state::DEAD;
			//	user->ChangeState(USER::state::DEAD);
			//}
			//shoot->ChangeState(false);
			ServerCollisionPacket.set_uid1(shoot->uid); ServerCollisionPacket.set_uid2(user->objectID); ServerCollisionPacket.set_hp(user->hp);
			//IocpConstructor::manageGame->removeBullet(ClientCollisionPacket.uid1());
			size = ServerCollisionPacket.ByteSize();

		}
		//총알 - 엔피씨
		else if (IocpConstructor::nm->mappingAI.count(ClientCollisionPacket.uid2()) != 0){
			std::shared_ptr<bullet> shoot = IocpConstructor::manageGame->retBullet(ClientCollisionPacket.uid1());
			std::shared_ptr<AI> ai = IocpConstructor::nm->retAI((int)ClientCollisionPacket.uid2());
			if (shoot == NULL || ai == NULL) return;
			//ai->hp -= shoot->damage;
			//if (ai->hp <= 0){
				//ai->current = AI::state::dead;
			//	ai->ChangeState(NPC_STATUS_DEAD);
		//	}
			//shoot->ChangeState(false);
			ServerCollisionPacket.set_uid1(shoot->uid); ServerCollisionPacket.set_uid2(ai->nid); ServerCollisionPacket.set_hp(ai->hp);
			//IocpConstructor::manageGame->removeBullet(ClientCollisionPacket.uid1());
			size = ServerCollisionPacket.ByteSize();
		}
		//총알 - 총알
		else{
			

			std::shared_ptr<bullet> shoot_1 = IocpConstructor::manageGame->retBullet(ClientCollisionPacket.uid1());
			shoot_1->ChangeState(false);
			//IocpConstructor::manageGame->removeBullet(ClientCollisionPacket.uid1());

			std::shared_ptr<bullet> shoot_2 = IocpConstructor::manageGame->retBullet(ClientCollisionPacket.uid2());
			shoot_2->ChangeState(false);
			//IocpConstructor::manageGame->removeBullet(ClientCollisionPacket.uid2());
			if (shoot_1 == NULL || shoot_2 == NULL) return;

			ServerCollisionPacket.set_uid1(ClientCollisionPacket.uid1()); ServerCollisionPacket.set_uid2(ClientCollisionPacket.uid2());
			size = ServerCollisionPacket.ByteSize();
		
		}
	}
	//총알 - 맵
	else if (IocpConstructor::manageGame->mappingBullet.count(ClientCollisionPacket.uid1())!=0)
	{
		std::shared_ptr<bullet> shoot = IocpConstructor::manageGame->retBullet(ClientCollisionPacket.uid1());
		if (shoot == NULL) return;
		shoot->ChangeState(false);
		//IocpConstructor::manageGame->removeBullet(ClientCollisionPacket.uid1());
		ServerCollisionPacket.set_uid1(shoot->uid);
		size = ServerCollisionPacket.ByteSize();
	}
	else return;

	type = PKT_S_COLLISION;
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	delete[] buffer;
	return;
}

void PacketHandler::S_MOVE_Handler(std::shared_ptr<AI> ai)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(ai->nid); ServerMovePacket.set_x(ai->x); ServerMovePacket.set_y(ai->y); ServerMovePacket.set_velocity(ai->velocity);
	ServerMovePacket.set_direction(ai->direction);
	size = ServerMovePacket.ByteSize();
	type = PKT_S_MOVE;
	
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerMovePacket.SerializePartialToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	delete[] buffer;
	return;

}

void PacketHandler::C_SHOOT_handler(Packet& p)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::C_Shoot ClientBullet;
	ClientBullet.ParseFromArray(p.Msg, p.getLength());
	std::shared_ptr<bullet> Bullet;
	{
		LOCKING(IocpConstructor::ObjectKey);
		std::shared_ptr<USER> tempUser = IocpConstructor::cm->retUser((int)ClientBullet.uid());
		std::shared_ptr<bullet> tempBullet(new bullet(IocpConstructor::ObjectCount, ClientBullet.uid(), tempUser->x, tempUser->y, 50.0f, 70.0f,ClientBullet.direction()));  
		Bullet = tempBullet;
		IocpConstructor::manageGame->registBullet(Bullet);
	}
	//총 움직임에 대한 'JOB'처리
	TimerJob job;
	job.exectime = GetTickCount() + 30;
	job.func = std::bind(&bullet::bulletMove, Bullet);  //만약에 안되면 여기 의심
	IocpConstructor::jobs.push_back(job);

	type = PKT_S_SHOOT;
	InGamePacket::S_Shoot ServerShootPacket;
	ServerShootPacket.set_bullet_uid(Bullet->uid);ServerShootPacket.set_x(Bullet->x); ServerShootPacket.set_y(Bullet->y);
	ServerShootPacket.set_uid(ClientBullet.uid());
	ServerShootPacket.set_damage(Bullet->damage); ServerShootPacket.set_direction(Bullet->direction); ServerShootPacket.set_velocity(Bullet->velocity);
	size = ServerShootPacket.ByteSize();

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerShootPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	delete[] buffer;
	return;
}

void PacketHandler::S_SHOOT_Handler(std::shared_ptr<bullet> b)
{

	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	InGamePacket::S_Shoot ServerShootPacket;
	
	ServerShootPacket.set_uid(b->shooter);  ServerShootPacket.set_bullet_uid(b->uid); ServerShootPacket.set_x(b->x); ServerShootPacket.set_y(b->y);
	ServerShootPacket.set_direction(b->direction); ServerShootPacket.set_velocity(b->velocity); ServerShootPacket.set_damage(b->damage);
	size = ServerShootPacket.ByteSize();
	type = PKT_S_SHOOT;

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerShootPacket.SerializePartialToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	delete[] buffer;
	return;
}

void PacketHandler::C_SHOOT_Handler(std::shared_ptr<bullet> b)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	InGamePacket::S_Shoot ServerShootPacket;
	ServerShootPacket.set_uid(b->uid); ServerShootPacket.set_x(b->x); ServerShootPacket.set_y(b->y);
	ServerShootPacket.set_direction(b->direction); ServerShootPacket.set_velocity(b->velocity); ServerShootPacket.set_damage(b->damage);
	size = ServerShootPacket.ByteSize();
	type = PKT_S_SHOOT;

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerShootPacket.SerializePartialToArray(buffer + sizeof(unsigned short)* 2, size);
	BroadCast(buffer, size + sizeof(unsigned short)* 2);

	delete[] buffer;
	return;
}

void PacketHandler::C_DISCONNECT_Handler(int id)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	AccountPacket::S_Account_List::Disconnect disconnect;
	type = PKT_S_DISCONNECT;
	disconnect.set_uid(id);
	size = disconnect.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	disconnect.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

//서버가 유저에게 보내는 총알 stop 
void PacketHandler::C_STOP_handler(std::shared_ptr<bullet> b)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Stop BulletStopPacket;
	BulletStopPacket.set_uid(b->uid);
	BulletStopPacket.set_x(b->x); BulletStopPacket.set_y(b->y);

	type = PKT_S_STOP;
	size = BulletStopPacket.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	BulletStopPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

//총알 - 벽
void PacketHandler::S_COLLISION_Handler(std::shared_ptr<bullet> b)
{
	if (b == nullptr) return;
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Collision BulletCollisionPacket;
	BulletCollisionPacket.set_uid1(b->uid);

	type = PKT_S_COLLISION;
	size =BulletCollisionPacket.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	BulletCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

//유저 - 벽
void PacketHandler::S_COLLISION_Handler(std::shared_ptr<USER> b)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Collision BulletCollisionPacket;
	BulletCollisionPacket.set_uid1(b->uid); BulletCollisionPacket.set_x(b->x); BulletCollisionPacket.set_y(b->y);

	type = PKT_S_COLLISION;
	size = BulletCollisionPacket.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	BulletCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

//유저 - 총알, 엔피시 - 총알
void PacketHandler::S_COLLISION_Handler(std::shared_ptr<bullet> b, int uid2, int _hp)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Collision BulletCollisionPacket;
	BulletCollisionPacket.set_uid1(b->uid); BulletCollisionPacket.set_uid2(uid2); BulletCollisionPacket.set_hp(_hp);


	type = PKT_S_COLLISION;
	size = BulletCollisionPacket.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	BulletCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

//서버가 유저에게 보내는 AI stop
void PacketHandler::S_STOP_Handler(std::shared_ptr<AI> ai)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Stop BulletStopPacket;
	BulletStopPacket.set_uid(ai->nid);
	BulletStopPacket.set_x(ai->x); BulletStopPacket.set_y(ai->y);

	type = PKT_S_STOP;
	size = BulletStopPacket.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	BulletStopPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

void PacketHandler::BroadCast(char *buffer, int size)
{
	ClientHandle tempHandle;
	std::map<int, std::shared_ptr<USER>>::iterator it;

	tempHandle.ioinfo = new IoData;
	memset(&tempHandle.ioinfo->overlapped, 0, sizeof(OVERLAPPED));
	tempHandle.ioinfo->wsaBuf.len = size;
	tempHandle.ioinfo->wsaBuf.buf = buffer;
	tempHandle.ioinfo->rwMode = WRITE;
	for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
	{
		if (it->second->connect){
			WSASend(it->second->uid, &(tempHandle.ioinfo->wsaBuf), 1, NULL, 0, &(tempHandle.ioinfo->overlapped), NULL);
		}
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
	else if (p.getType() == PKT_C_SHOOT){
		C_SHOOT_handler(p);
	}
	else if (p.getType() == PKT_C_COLLISION){
		C_COLLISION_Handler(p);
	}
	else
	{
		return false;
	}
	return true;
}
