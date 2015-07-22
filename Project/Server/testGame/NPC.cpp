#include "stdafx.h"
#include "NPC.h"


NPC::NPC()
{
	this->x = 0;
	this->y = 0;
	this->LIFE = 100;
	this->th = 0;
	this->Key = new Lock;
}

NPC::NPC(int _x, int _y, int _life, int _th)
{
	this->x = _x;
	this->y = _y;
	this->LIFE = _life;
	this->th = _th;
	this->Key = new Lock;
}

NPC::~NPC()
{
}

void NPC::Move()
{
	LOCKING(this->Key);
	this->x += 4; this->y += 6;
	printf("%d MOVE %d %d\n", this->th, this->x, this->y);
	return;
}

void NPC::Attack(std::map<SOCKET, USER> t)
{
	if (t.empty()){
		printf("접속한 유저가 없습니다\n");
		return;
	}
	std::map<SOCKET, USER>::iterator it;
	for (it = t.begin(); it != t.end(); ++it){
		

	}
	printf("%d Attack\n", this->th);
}