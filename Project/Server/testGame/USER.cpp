#include "stdafx.h"
#include "USER.h"


USER::USER()
{
	this->x = 10.0f;
	this->y = 10.0f;
	this->hp = 100;
	this->current = 0;
	this->total = 0;
	this->state = WAIT;
	this->velocity = 0.0f;
	this->key = new Lock();
}

USER::USER(int _id, int _hp, float _x, float _y)
{
	this->uid = _id;
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
	this->current = 0;
	this->total = 0;
	this->state = WAIT;
	this->velocity = 0.0f;
	this->key = new Lock();
}

USER::~USER()
{
}


void USER::setUSER(int _hp, float _x, float _y)
{
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
}

void USER::setTotal(int len)
{
	this->total = len;
	return;
}

void USER::setCurrent(int len)
{
	this->current = len;
	return;
}

int USER::getTotal()
{
	return this->total;
}

int USER::getCurrent()
{
	return this->current;
}

void USER::setBuffer(char *msg, int len)
{
	memcpy(this->Buffer, msg, len);
	return;
}

char* USER::getBuffer()
{
	return this->Buffer;
}


bool USER::finished()
{
	if (this->current == this->total && this->current != 0) return true;
	return false;
}

void USER::clear()
{
	this->current = 0;
	this->total = 0;
	memset(this->Buffer, 0, sizeof(this->Buffer));
}

void USER::UserpacketHandle()
{
	Packet userPacket;
	unsigned short current = 0;

	//계속해서 유저를 처리할 수 있도록
	LockHelper(this->key);
	while ( current < this->getTotal()){
		memcpy(&userPacket.Length, this->Buffer+current, sizeof(current));
		current += sizeof(unsigned short);
		memcpy(&userPacket.Type, this->Buffer + current, sizeof(current));
		current += sizeof(unsigned short);
		memcpy(userPacket.Msg, this->Buffer + current, userPacket.Length);
		current += userPacket.Length;
		PacketHandler::GetInstance()->HandlePacket(userPacket);
	}
	//'clear'의 위치를 잘 생각해주어함.....
	this->clear();
}

void USER::UserMove(){

	TimerJob userMoveJob;
	//시간초보다 작으면 수행되겠지..잡큐에서 알아서 수행해줌
	if (this->state == MOVE){
		//현재위치 갱신과 위치를 위치를 브로드캐스팅
		this->x += (this->velocity * 0.1f);
		PacketHandler::GetInstance()->C_MOVE_Handler(*this);


		//움직일 작업에 대해서 처리..
		userMoveJob.func = std::bind(&USER::UserMove, this);
		userMoveJob.exectime = GetTickCount() + 100;
		IocpConstructor::jobs.push_back(userMoveJob);
	}
	return;
}