#include "stdafx.h"
#include "BULLET.h"


BULLET::BULLET()
{
	this->key = new Lock();
	this->damage = BULLET_DAMAGE_1;
	this->type = Object_BULLET;
	this->ay = GRAVITY;
	this->ax = 0;
}


BULLET::~BULLET()
{
	delete this->key;
}


BULLET::BULLET(float _x, float _y, float _vx, float _vy, int _shooter) : BULLET()
{
	this->x = _x;
	this->y = _y;
	this->vx = _vx * BULLET_VELOCITY;
	this->vy = _vy * BULLET_VELOCITY;
	this->shooter = _shooter;
}

void BULLET::BULLET_MOVE()
{
	LOCKING(this->key);
	if (this->CurrentState == BULLET::state::ABLE){
		
		float tempVx = this->vx + (0.03f * 0.03f * this->ax / 2);
		float tempVy = this->vy + (0.03f * 0.03f * this->ay / 2);
		//�̺κ� �����ؾ���...
		float dx = this->x + this->vx * 0.03f;
		float dy = this->y + this->vy * 0.03f;

		std::shared_ptr<BULLET> Bullet = std::static_pointer_cast<BULLET>(IocpConstructor::Object_Manager->FIND(this->ObjectId));
		//���Ⱑ �ϴ� �浹ó���Ѵ� �κ���....
		//�ʰ��� �浹ó���� �������� �浹ó���� �ʿ��ϴ�...�ٿ����üũ..
		if (dx >= WIDTH || dx < 0 || dy >= HEIGHT || dy < 0)
		{
			//�Ѿ��� ������� ��..
			//stop ��Ŷ�� ��������
			this->CurrentState = BULLET::state::DISABLE;
			////�̺κе�����.~
			PacketHandler::GetInstance()->S_COLLISION_HANDLER(Bullet,nullptr);
			IocpConstructor::Object_Manager->REMOVE(Bullet->ObjectId);
			return;
		}

		for (auto ob : IocpConstructor::Object_Manager->OBJECT_MAP)
		{
			float o_dx = ob.second->x; float o_dy = ob.second->y;
			if (sqrt((dx - o_dx)*(dx - o_dx) + (dy - o_dy)*(dy - o_dy)) < DAMAGE_DISTANCE)
			{
				//����� ������ ���
				if (ob.second->type == Object_USER){
					std::shared_ptr<USER> User = std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(ob.second->ObjectId));
					if ((this->shooter != ob.second->ObjectId) && ((User->CurrentState == USER::state::ALIVE) || User->CurrentState == USER::state::STOP))
					{
						User->USER_SET_HP(User->hp -= this->damage);
						if (User->hp <= 0)
						{
							User->ChangeState(USER::state::DEAD);
						}
						PacketHandler::GetInstance()->S_COLLISION_HANDLER(Bullet, User);
						IocpConstructor::Object_Manager->REMOVE(Bullet->ObjectId);
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
					}

				}
				else if (ob.second->type == Object_BULLET)
				{
					std::shared_ptr<BULLET> Bullet_2 = std::static_pointer_cast<BULLET>(IocpConstructor::Object_Manager->FIND(ob.second->ObjectId));
					PacketHandler::GetInstance()->S_COLLISION_HANDLER(Bullet, Bullet_2);
					IocpConstructor::Object_Manager->REMOVE(Bullet->ObjectId);
					IocpConstructor::Object_Manager->REMOVE(Bullet_2->ObjectId);
				}
			}
		}
		TimerJob bulletMoveJob;
		this->x = dx;
		this->y = dy;
		//�����ε� ĳ�����ϴ� �κ��� ����..
		//PacketHandler::GetInstance()->C_SHOOT_Handler(bul);
		bulletMoveJob.func = std::bind(&BULLET::BULLET_MOVE, Bullet);
		bulletMoveJob.exectime = GetTickCount() + 30;
		{
			LOCKING(IocpConstructor::queueLock)
			IocpConstructor::jobs.push_back(bulletMoveJob);
		}

	}
	return;
}

void BULLET::CHANGE_STATE(int _state)
{
	LOCKING(this->key);
	this->CurrentState = BULLET::state(_state);
	return;

}