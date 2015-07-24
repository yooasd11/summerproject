#include "stdafx.h"
#include "bullet.h"


bullet::bullet(int _th, int _id, float _x, float _y, float _damage, float _v, float _direction)
{
	this->key = new Lock();
	this->th = _th;
	this->uid = _id;
	this->x = _x;
	this->y = _y;
	this->velocity = _v;
	this->direction = _direction;
	this->damage = _damage;

}

bullet::bullet()
{
	this->key = new Lock();
}


bullet::~bullet()
{
}

void bullet::bulletMove()
{
	//총알이 언제 안보여야 되는지 알아야함...640 X 320
	if (x >= 640.0f || x < 0 || y >= 320.f || y < 0)
	{
		//총알을 지워줘야 함..
		IocpConstructor::manageGame->removeBullet(this->th);
	}
	else 
	{
		//유저의 위치랑 일치하는지 확인해야함..공격범위를 어느정도까지 ? 
		std::map<SOCKET, std::shared_ptr<USER>>::iterator it;
		for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
		{
			float userX = it->second->x; float userY = it->second->y;


		}

		TimerJob bulletMoveJob;
		this->x += (this->velocity * 0.1f * cos(this->direction * PI / 180));
		this->y += (this->velocity * 0.1f * sin(this->direction * PI / 180));
		PacketHandler::GetInstance()->C_SHOOT_Handler(IocpConstructor::manageGame->retBullet(this->th));
		

		bulletMoveJob.func = std::bind(&bullet::bulletMove, this);
		bulletMoveJob.exectime = GetTickCount() + 100;
		IocpConstructor::jobs.push_back(bulletMoveJob);
	}
	return;
}