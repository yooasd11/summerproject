#include "stdafx.h"
#include "bullet.h"


bullet::bullet(int _id, int _shooter,  float _x, float _y, float _damage, float _v, float _direction)
{
	this->key = new Lock();
	this->uid = _id;
	this->x = _x;
	this->y = _y;
	this->velocity = _v;
	this->direction = _direction;
	this->damage = _damage;
	this->shooter = _shooter;
	this->working = true;
}

bullet::bullet()
{
	this->key = new Lock();
}

bullet::~bullet()
{
	delete this->key;
}

void bullet::ChangeState(bool s)
{
	LOCKING(this->key);
	if (!this->working)
		this->working = s;
	return;
}

void bullet::bulletMove()
{
	//존재하는 총알에 대한 작업을 처리 
	LOCKING(this->key);
	if (this->working){
		//총알이 언제 안보여야 되는지 알아야함...640 X 320
		float dx = this->x + (this->velocity * 0.03f * sin(this->direction * PI / 180));
		float dy = this->y + (this->velocity * 0.03f * cos(this->direction * PI / 180));
		//printf("%f %f\n", dx, dy);
		std::shared_ptr<bullet> bul = IocpConstructor::manageGame->retBullet(this->uid);
		//여기가 일단 충돌처리한느 부분임....
		//맵과의 충돌처리와 유저와의 충돌처리가 필요하다...바운더리체크..
		if (dx >= 1500.0f || dx < 0 || dy >= 700.0f || dy < 0)
		{
			//총알을 지워줘야 함..
			//stop 패킷을 보내야함
			this->working = false;
			PacketHandler::GetInstance()->S_COLLISION_Handler(bul);
			IocpConstructor::manageGame->removeBullet(bul->uid);
			return;
		}

		for (auto user : IocpConstructor::cm->mappingClient)
		{
			float userX = user.second->x; float userY = user.second->y;
			if (sqrt((dx - userX)*(dx - userX) + (dy - userY)*(dy - userY)) < DAMAGE_DISTANCE && user.second->objectID != this->shooter && (user.second->crt != USER::state::DEAD))
			{
				this->working = false;
				user.second->hp -= this->damage;
				if (user.second->hp <= 0)
				{
					printf("죽었음\n");
					user.second->ChangeState(USER::state::DEAD);
					printf("%d\n", user.second->crt);
				}
				//데미지를 입은부분 전달
				PacketHandler::GetInstance()->S_COLLISION_Handler(bul, user.second->objectID, user.second->hp);
				IocpConstructor::manageGame->removeBullet(bul->uid);

				return;
			}
		}

		for (auto AI :IocpConstructor::nm->mappingAI )
		{
			float aiX = AI.second->x; float aiY = AI.second->y;
			if (sqrt((dx - aiX)*(dx - aiX) + (dy - aiY)*(dy - aiY)) < DAMAGE_DISTANCE && AI.second->nid != this->shooter && AI.second->current_state != AI.second->npc_dead)
			{
				this->working = false;
				AI.second->hp -= this->damage;
				if (AI.second->hp <= 0)
				{
					AI.second->ChangeState(NPC_STATUS_DEAD);
				}
				PacketHandler::GetInstance()->S_COLLISION_Handler(bul, AI.second->nid, AI.second->hp);
				IocpConstructor::manageGame->removeBullet(bul->uid);
				return;
			}
		}
		TimerJob bulletMoveJob;
		this->x = dx;
		this->y = dy;
		//현재브로드 캐스팅하는 부분을 지움..
		//PacketHandler::GetInstance()->C_SHOOT_Handler(bul);
		bulletMoveJob.func = std::bind(&bullet::bulletMove, bul);
		bulletMoveJob.exectime = GetTickCount() + 30;
		{
			LOCKING(IocpConstructor::queueLock)
			IocpConstructor::jobs.push_back(bulletMoveJob);
		}
		
	}
	return;
}