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
	//존재하는 총알에 대한 작업을 처리
	if (this->working){
		//총알이 언제 안보여야 되는지 알아야함...640 X 320
		float dx = this->x + (this->velocity * 0.1f * sin((this->direction + 180.0f) * PI / 180));
		float dy = this->y + (this->velocity * 0.1f * cos(this->direction * PI / 180));

		//여기가 일단 충돌처리한느 부분임....
		//맵과의 충돌처리와 유저와의 충돌처리가 필요하다...바운더리체크..
		if (dx >= 640.0f || dx < 0 || dy >= 320.0f || dy < 0)
		{
			//총알을 지워줘야 함..
			//stop 패킷을 보내야함
			this->working = false;
			PacketHandler::GetInstance()->S_COLLISION_Handler(IocpConstructor::manageGame->retBullet(this->th));
			IocpConstructor::manageGame->removeBullet(IocpConstructor::manageGame->retBullet(this->th)->th);
			return;
		}

		//유저의 위치랑 일치하는지 확인해야함..공격범위를 어느정도까지 ? 
		std::map<SOCKET, std::shared_ptr<USER>>::iterator it;
		for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
		{
			//유저가 맞았을 경우..
			float userX = it->second->x; float userY = it->second->y;
			if ( sqrt( (dx - userX)*(dx - userX) + (dy - userY)*(dy - userY) ) < 20 && it->second->uid != this->uid)
			{
				//유저의 데미지 감소..
				printf("damage\n");
				this->working = false;
				it->second->hp -= this->damage;
				PacketHandler::GetInstance()->S_COLLISION_Handler(IocpConstructor::manageGame->retBullet(this->th), it->first, it->second->hp);
				IocpConstructor::manageGame->removeBullet(IocpConstructor::manageGame->retBullet(this->th)->th);
				return;			
			}
		}

		TimerJob bulletMoveJob;
		this->x = dx;
		this->y = dy;
		PacketHandler::GetInstance()->C_SHOOT_Handler(IocpConstructor::manageGame->retBullet(this->th));

		bulletMoveJob.func = std::bind(&bullet::bulletMove, this);
		bulletMoveJob.exectime = GetTickCount() + 30;
		IocpConstructor::jobs.push_back(bulletMoveJob);
	}
	return;
}