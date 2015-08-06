#include "stdafx.h"
#include "BULLET.h"


BULLET::BULLET()
{
	this->damage = BULLET_DAMAGE_1;
	this->type = Object_BULLET;
	this->ay = GRAVITY;
	this->ax = 0;
	this->CurrentState = BULLET::state::ABLE;
}


BULLET::~BULLET()
{
	
}


BULLET::BULLET(float _x, float _y, float _vx, float _vy, int _shooter) : BULLET()
{
	this->x = _x;
	this->y = _y;
	this->vx = _vx;
	this->vy = _vy;
	this->shooter = _shooter;
}

void BULLET::BULLET_MOVE()
{
	LOCKING(&this->key);
	if (this->CurrentState == BULLET::state::ABLE){
		
		float t_vx = this->vx + this->ax * 0.03f;
		float t_vy = this->vy + this->ay * 0.03f;

		float t_x = this->x + t_vx * 0.03f;
		float t_y = this->y + t_vy * 0.03f;
		
		std::shared_ptr<BULLET> Bullet = std::static_pointer_cast<BULLET>(IocpConstructor::Object_Manager->FIND(this->ObjectId));
	
		//여기가 일단 충돌처리한느 부분임....
		//맵과의 충돌처리와 유저와의 충돌처리가 필요하다...바운더리체크..
		if (t_x >= WIDTH || t_x < 0 || t_y >= HEIGHT || t_y < BULLET_LAND)
		{
			this->CurrentState = BULLET::state::DISABLE;
			PacketHandler::GetInstance()->S_COLLISION_HANDLER(Bullet,nullptr);
			IocpConstructor::Object_Manager->REMOVE(Bullet->ObjectId);
			return;
		}

		std::map<int, std::shared_ptr<OBJECT>> Instance_map;
		{
			LOCKING(&IocpConstructor::ObjectKey);
			Instance_map = IocpConstructor::Object_Manager->OBJECT_MAP;
		}

		for (auto ob : Instance_map)
		{
			if (ob.second == nullptr) continue;
			float o_dx = ob.second->x; float o_dy = ob.second->y;
			if (sqrt((t_x - o_dx)*(t_x - o_dx) + (t_y - o_dy)*(t_y - o_dy)) < DAMAGE_DISTANCE)
			{
				//대상이 유저일 경우
				if (ob.second->type == Object_USER){
					std::shared_ptr<USER> User = std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(ob.second->ObjectId));
					if ((this->shooter != ob.second->ObjectId) && ((User->CurrentState == USER::state::ALIVE) || User->CurrentState == USER::state::STOP))
					{
						User->USER_SET_HP(User->hp -= this->damage);
						if (User->hp <= 0)
						{
							User->ChangeState(USER::state::DEAD);
							TimerJob job;
							job.exectime = GetTickCount() + 5000;
							job.func = std::bind(&USER::USER_RESPAWN, User);
							{
								LOCKING(&IocpConstructor::queueLock);
								IocpConstructor::jobs.push_back(job);
							}
						}
						PacketHandler::GetInstance()->S_COLLISION_HANDLER(Bullet, User);
						IocpConstructor::Object_Manager->REMOVE(Bullet->ObjectId);
						return;
					}
				}
				else if (ob.second->type == Object_NPC)
				{
					std::shared_ptr<NPC> Npc = std::static_pointer_cast<NPC>(IocpConstructor::Object_Manager->FIND(ob.second->ObjectId));
					if ((this->shooter != ob.second->ObjectId) && (Npc->current_state == Npc->npc_alive))
					{
						Npc->NPC_SET_HP(Npc->hp -= this->damage);
						if (Npc->hp <= 0)
						{
							Npc->NPC_STATE_CHANGE(NPC_STATUS_DEAD);
						}
						
						PacketHandler::GetInstance()->S_COLLISION_HANDLER(Bullet, Npc);
						IocpConstructor::Object_Manager->REMOVE(Bullet->ObjectId);
						return;
					}

				}
				//else if (ob.second->type == Object_BULLET && this->ObjectId != ob.second->ObjectId)
				//{
				//	std::shared_ptr<BULLET> Bullet_2 = std::static_pointer_cast<BULLET>(IocpConstructor::Object_Manager->FIND(ob.second->ObjectId));
				//	PacketHandler::GetInstance()->S_COLLISION_HANDLER(Bullet, Bullet_2);
				//	//IocpConstructor::Object_Manager->REMOVE(Bullet->ObjectId);
				//	IocpConstructor::Object_Manager->REMOVE(Bullet_2->ObjectId);
				//	return;
				//}
			}
		}
		TimerJob bulletMoveJob;
		this->vx = t_vx;
		this->vy = t_vy;
		this->x = t_x;
		this->y = t_y;
		//현재브로드 캐스팅하는 부분을 지움..
		//PacketHandler::GetInstance()->S_SHOOT_HANDLER(Bullet);
		
		Bullet = std::static_pointer_cast<BULLET>(IocpConstructor::Object_Manager->FIND(this->ObjectId));
		if (Bullet == nullptr) return;
		bulletMoveJob.func = std::bind(&BULLET::BULLET_MOVE, std::static_pointer_cast<BULLET>(IocpConstructor::Object_Manager->FIND(Bullet->ObjectId)));
		bulletMoveJob.exectime = GetTickCount() + 30;
		{
			LOCKING(&IocpConstructor::queueLock)
			IocpConstructor::jobs.push_back(bulletMoveJob);
		}
	}
	return;
}

void BULLET::CHANGE_STATE(int _state)
{
	LOCKING(&this->key);
	this->CurrentState = BULLET::state(_state);
	return;

}