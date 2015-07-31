#include "stdafx.h"
#include "AI.h"


AI::AI()
{
	this->npc_dead = new NPC_DEAD();
	this->npc_alive = new NPC_ALIVE();
	this->key = new Lock();
}

AI::AI(int _nid, int _hp, float _x, float _y, float _direction, float _velocity)
{
	this->npc_dead = new NPC_DEAD();
	this->npc_alive = new NPC_ALIVE();
	this->current_state = npc_alive;
	this->nid = _nid;
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
	this->direction = _direction;
	this->velocity = _velocity;
	this->key = new Lock();
}

AI::~AI()
{
	delete this->key;
}

float AI::getDegree(float x1, float y1, float x2, float y2)
{
	return -(atan2(y2-y1, x2-x1)-90.0f);
}

//�̵��Լ��� ���� �ֳ� ���� �Ǵ��ϰ� , �ֺ��� �ִٰ� �Ǵ����� ��� attack�� �õ�..
//attack �Լ��� attack�� 

void AI::ChangeDirection()
{
	LOCKING(this->key);
	this->direction *= -1;
	return;
}

void AI::ChangeState(int s)
{
	LOCKING(this->key);
	if (this->current_state == this->npc_dead) return;
	if (s == NPC_STATUS_ALIVE){
		this->current_state = this->npc_alive;
	}
	else if (s == NPC_STATUS_DEAD){
		this->current_state = this->npc_dead;
	}
	return;
}


void AI::init()
{
	TimerJob job, job2;
	job.exectime = GetTickCount() + NEXT_TICK;
	job2.exectime = GetTickCount() + AI_DIRECTION_DELAY;
	job.func = std::bind(&AI::decision, IocpConstructor::nm->retAI(this->nid));
	job2.func = std::bind(&AI::AI_Change_Direction, IocpConstructor::nm->retAI(this->nid));
	{
		LOCKING(IocpConstructor::queueLock)
		IocpConstructor::jobs.push_back(job);
		IocpConstructor::jobs.push_back(job2);
	}
	return;
}

void AI::decision()
{
	LOCKING(this->key);
	this->current_state->NPC_Action(IocpConstructor::nm->retAI(this->nid));
	return;
}

void AI::AI_Change_Direction()
{
	if (this->current_state == this->npc_dead) return;
	this->ChangeDirection();
	TimerJob job;
	job.exectime = GetTickCount() + AI_DIRECTION_DELAY;
	job.func = std::bind(&AI::AI_Change_Direction, IocpConstructor::nm->retAI(this->nid));
	{
		LOCKING(IocpConstructor::queueLock);
		IocpConstructor::jobs.push_back(job);
	}
	return;
}

void AI::AI_set_hp(int _hp){
	LOCKING(this->key);
	this->hp = _hp;
	return;
}
//void AI::Action()
//{
//	LOCKING(this->key);
//	//����ִ� ������ AI�� �ൿ����
//	if (this->current == state::dead) return;
//	if (this->current == state::alive)
//	{
//		printf("%d\n", this->nid);
//		std::map<int, std::shared_ptr<USER>>::iterator it;
//		std::shared_ptr<bullet> Bullet;
//		
//		for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
//		{
//			float dx = it->second->x; float dy = it->second->y;
//			
//			//�����Ÿ� ������ �� ������ �Ѵ�...�Ѿ��� �����ؼ� �������� 
//			if (sqrt((this->x - dx)*(this->x - dx) + (this->y - dy)*(this->y - dy)) < DISTANCE )
//			{
//
//				float degree = getDegree(this->x, this->y, dx, dy);
//				this->current = state::Waiting;
//				{
//					LOCKING(IocpConstructor::ObjectKey);
//					std::shared_ptr<bullet> tempBullet(new bullet(IocpConstructor::ObjectCount, this->nid, this->x, this->y,
//						BULLET_DAMAGE_1, BULLET_VELOCITY, degree));
//					Bullet = tempBullet;
//					IocpConstructor::manageGame->registBullet(Bullet);
//					//�Ѿ��� �����ϰ� �߻�..�ٵ� ������ ����
//					//�߻��ϰ� 1���Ŀ� �۾��� �ٽ� ����ǵ��� ����...
//				}
//				//�Ѿ˰� AI�� ���� ��� �۾��� �־������..
//				TimerJob job, job2;
//				job.exectime = GetTickCount() + AI_NEXT_TICK;
//				job.func = std::bind(&AI::Wait, IocpConstructor::nm->retAI(this->nid));
//				job2.exectime = GetTickCount() + NEXT_TICK;
//				job2.func = std::bind(&bullet::bulletMove, Bullet);
//				{
//					LOCKING(IocpConstructor::queueLock)
//					IocpConstructor::jobs.push_back(job);
//					IocpConstructor::jobs.push_back(job2);
//				}
//				PacketHandler::GetInstance()->S_STOP_Handler(IocpConstructor::nm->retAI(this->nid));
//				PacketHandler::GetInstance()->S_SHOOT_Handler(Bullet);
//				return;
//			}
//		}
//		//������ ���� ��� �̵�...�̷��� �Ǹ� �������θ� �����̰� ��..�̵��� ���� �浹ó��
//		this->x = this->x + (this->velocity * 0.03f * sin(this->direction * PI / 180));
//		this->y = this->y + (this->velocity * 0.03f * cos(this->direction * PI / 180));
//		TimerJob job;
//		job.exectime = GetTickCount() + NEXT_TICK;
//		job.func = std::bind(&AI::Action, IocpConstructor::nm->retAI(this->nid));
//		{
//			LOCKING(IocpConstructor::queueLock)
//			IocpConstructor::jobs.push_back(job);
//		}
//		PacketHandler::GetInstance()->S_MOVE_Handler(IocpConstructor::nm->retAI(this->nid));
//	}
//	return;
//}