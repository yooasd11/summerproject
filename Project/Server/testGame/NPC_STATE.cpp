#include "stdafx.h"

#include "NPC_STATE.h"

#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)


float getDegree(float x1, float y1, float x2, float y2)
{
	return MATH_RAD_TO_DEG(atan2(x2 - x1, y2 - y1));
}

NPC_STATE::NPC_STATE()
{

}

NPC_STATE::~NPC_STATE()
{

}

NPC_ALIVE::NPC_ALIVE(){


}
NPC_DEAD::NPC_DEAD(){

}

void NPC_STATE::NPC_Action(std::shared_ptr<AI> ai)
{

}
void NPC_DEAD::NPC_Action(std::shared_ptr<AI> ai)
{

}



void NPC_ALIVE::NPC_Action(std::shared_ptr<AI> ai)
{
	//공격을 했음..
	std::shared_ptr<bullet> Bullet;
	for (auto it : IocpConstructor::cm->mappingClient)
	{
		float dx = it.second->x; float dy = it.second->y;
		if ((sqrt((ai->x - dx)*(ai->x - dx) + (ai->y - dy)*(ai->y - dy)) < DISTANCE) && (it.second->crt != USER::state::DEAD))
		{
			float degree = getDegree(ai->x, ai->y, dx, dy);
			{
				LOCKING(IocpConstructor::ObjectKey);
				std::shared_ptr<bullet> tempBullet(new bullet(IocpConstructor::ObjectCount, ai->nid, ai->x, ai->y,
					BULLET_DAMAGE_1, BULLET_VELOCITY, degree));
				Bullet = tempBullet;
				IocpConstructor::manageGame->registBullet(Bullet);
			}
			TimerJob job, job2;
			job.exectime = GetTickCount() + AI_ATTACK_DELAY;
			job.func = std::bind(&AI::decision, IocpConstructor::nm->retAI(ai->nid));
			job2.exectime = GetTickCount() + NEXT_TICK;
			job2.func = std::bind(&bullet::bulletMove, Bullet);
			{
				LOCKING(IocpConstructor::queueLock)
				IocpConstructor::jobs.push_back(job);
				IocpConstructor::jobs.push_back(job2);
			}
			PacketHandler::GetInstance()->S_STOP_Handler(IocpConstructor::nm->retAI(ai->nid));
			PacketHandler::GetInstance()->S_SHOOT_Handler(Bullet);
			return;
		}
	}
	ai->x = ai->x + (ai->velocity * 0.03f * sin(ai->direction * PI / 180));
	ai->y = ai->y + (ai->velocity * 0.03f * cos(ai->direction * PI / 180));
	TimerJob job;
	job.exectime = GetTickCount() + AI_MOVE_DELAY;
	job.func = std::bind(&AI::decision, IocpConstructor::nm->retAI(ai->nid));
	{
		LOCKING(IocpConstructor::queueLock);
		IocpConstructor::jobs.push_back(job);
	}
	PacketHandler::GetInstance()->S_MOVE_Handler(IocpConstructor::nm->retAI(ai->nid));
	return;
}