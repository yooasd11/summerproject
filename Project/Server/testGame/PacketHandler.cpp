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

	//std::map<SOCKET, USER>::iterator it;
	std::map<SOCKET, std::shared_ptr<USER>>::iterator it;

	
	//패킷을 만드는 과정...데이터를 읽고 있는데 변환되면 안된다. 'LOCK'이 필요한가
	for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
	{
		AccountPacket::S_Account_List::Account *tempAccount = tempList.add_account_member();
		tempAccount->set_uid(it->second->uid); tempAccount->set_hp(it->second->hp); tempAccount->set_x(it->second->x); tempAccount->set_y(it->second->y);
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
	//USER* user = IocpConstructor::cm->retUser(MovePacket.uid());


	//유저를 불러와서 만약 연결이 끊어진 상태라면 패킷을 전송하지 않고 종료
	std::shared_ptr<USER> user = IocpConstructor::cm->retUser(MovePacket.uid());
	if (!user->connect) return;

	//유저정보 수정...
	user->state = MOVE;
	user->velocity = MovePacket.velocity();
	user->direction = MovePacket.direction();
	type = PKT_S_MOVE;

	
	
	//최초의 작업을 잡큐에 넣어줌..
	TimerJob job;
	job.current = job.state::UserMove;
	job.exectime = GetTickCount() + 100;
	job.func = std::bind(&USER::UserMove, user);
	IocpConstructor::jobs.push_back(job);

	
	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(MovePacket.uid()); ServerMovePacket.set_velocity(MovePacket.velocity());
	ServerMovePacket.set_x(user->x); ServerMovePacket.set_y(user->y); ServerMovePacket.set_direction(MovePacket.direction());
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

void PacketHandler::C_MOVE_Handler(std::shared_ptr<USER> user)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type, current = 0;

	InGamePacket::S_Move ServerMovePacket;
	ServerMovePacket.set_uid(user->uid); ServerMovePacket.set_x(user->x); ServerMovePacket.set_y(user->y); ServerMovePacket.set_velocity(user->velocity);
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
	receiveType = StopPacket.type();
	InGamePacket::S_Stop ServerStopPacket;
	if (receiveType == PLAYER)
	{
		std::shared_ptr<USER> user = IocpConstructor::cm->retUser(StopPacket.uid());
		user->state = WAIT;
		ServerStopPacket.set_uid(StopPacket.uid()); ServerStopPacket.set_x(user->x); ServerStopPacket.set_y(user->y);
		ServerStopPacket.set_type(PLAYER);
		size = ServerStopPacket.ByteSize();
	}
	else if (receiveType == BULLET)
	{
		//총알 멈추는 처리...
		std::shared_ptr<bullet> shoot = IocpConstructor::manageGame->retBullet(StopPacket.th());
		IocpConstructor::manageGame->removeBullet(StopPacket.th());
		shoot->working = false;
		ServerStopPacket.set_uid(shoot->uid); ServerStopPacket.set_type(BULLET); ServerStopPacket.set_x(shoot->x); ServerStopPacket.set_y(shoot->y);
		ServerStopPacket.set_th(shoot->th);
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

	std::shared_ptr<bullet> shoot = IocpConstructor::manageGame->retBullet(ClientCollisionPacket.th());
	if (shoot == NULL) return;

	IocpConstructor::manageGame->removeBullet(ClientCollisionPacket.th());
	shoot->working = false;
	ServerCollisionPacket.set_uid1(shoot->uid); ServerCollisionPacket.set_th(shoot->th); ServerCollisionPacket.set_x(shoot->x);
	ServerCollisionPacket.set_y(shoot->y);
	size = ServerCollisionPacket.ByteSize();
	
	type = PKT_S_STOP;
	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
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

	std::shared_ptr<bullet> Bullet(new bullet(IocpConstructor::manageGame->bulletCount, ClientBullet.uid(), ClientBullet.x(), ClientBullet.y(),
		ClientBullet.damage(), ClientBullet.velocity(), ClientBullet.direction()));

	Bullet->working = true;
	IocpConstructor::manageGame->registBullet(Bullet);

	//총 움직임에 대한 'JOB'처리
	TimerJob job;
	job.exectime = GetTickCount() + 30;
	job.func = std::bind(&bullet::bulletMove, Bullet);  //만약에 안되면 여기 의심
	IocpConstructor::jobs.push_back(job);

	type = PKT_S_SHOOT;
	InGamePacket::S_Shoot ServerShootPacket;
	ServerShootPacket.set_uid(Bullet->uid); ServerShootPacket.set_th(Bullet->th); ServerShootPacket.set_x(Bullet->x); ServerShootPacket.set_y(Bullet->y);
	ServerShootPacket.set_damage(Bullet->damage); ServerShootPacket.set_direction(Bullet->direction); ServerShootPacket.set_velocity(Bullet->velocity);
	size = ServerShootPacket.ByteSize();

	memcpy(buffer, &size, sizeof(size));
	memcpy(buffer + sizeof(size), &type, sizeof(type));
	ServerShootPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);
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
	ServerShootPacket.set_uid(b->uid); ServerShootPacket.set_th(b->th); ServerShootPacket.set_x(b->x); ServerShootPacket.set_y(b->y);
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

void PacketHandler::C_DISCONNECT_Handler(SOCKET sock)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	AccountPacket::S_Account_List::Disconnect disconnect;
	type = PKT_S_DISCONNECT;
	disconnect.set_uid(sock);
	size = disconnect.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	disconnect.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

//서버가 유저에게 보내는 s
void PacketHandler::C_STOP_handler(std::shared_ptr<bullet> b)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Stop BulletStopPacket;
	BulletStopPacket.set_uid(b->uid); BulletStopPacket.set_th(b->th); BulletStopPacket.set_type(BULLET);
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

void PacketHandler::S_COLLISION_Handler(std::shared_ptr<bullet> b)
{
	if (b == nullptr) return;
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Collision BulletCollisionPacket;
	BulletCollisionPacket.set_uid1(b->uid); BulletCollisionPacket.set_th(b->th); BulletCollisionPacket.set_x(b->x); BulletCollisionPacket.set_y(b->y);

	type = PKT_S_COLLISION;
	size =BulletCollisionPacket.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	BulletCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

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

void PacketHandler::S_COLLISION_Handler(std::shared_ptr<bullet> b, int uid2, int _hp)
{
	char* buffer = new char[BUFSIZE];
	memset(buffer, 0, sizeof(buffer));
	unsigned short size = 0, type = 0;

	InGamePacket::S_Collision BulletCollisionPacket;
	BulletCollisionPacket.set_uid1(b->uid); BulletCollisionPacket.set_th(b->th); BulletCollisionPacket.set_uid2(uid2); BulletCollisionPacket.set_hp(_hp);
	BulletCollisionPacket.set_x(b->x); BulletCollisionPacket.set_y(b->y);

	type = PKT_S_COLLISION;
	size = BulletCollisionPacket.ByteSize();

	memcpy(buffer, &size, sizeof(unsigned short));
	memcpy(buffer + sizeof(unsigned short), &type, sizeof(unsigned short));
	BulletCollisionPacket.SerializeToArray(buffer + sizeof(unsigned short)* 2, size);

	BroadCast(buffer, size + sizeof(unsigned short)* 2);
	delete[] buffer;
	return;
}

void PacketHandler::BroadCast(char *buffer, int size)
{

	ClientHandle tempHandle;
	//std::map<SOCKET, USER>::iterator it;
	std::map<SOCKET, std::shared_ptr<USER>>::iterator it;

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
	//


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
