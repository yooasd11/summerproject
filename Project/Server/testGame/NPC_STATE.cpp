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

NPC_ALIVE::NPC_ALIVE()
{


}

NPC_ALIVE::~NPC_ALIVE()
{


}
NPC_DEAD::NPC_DEAD(){

}

NPC_DEAD::~NPC_DEAD(){

}
void NPC_STATE::NPC_Action(std::shared_ptr<NPC> Npc)
{

}
void NPC_DEAD::NPC_Action(std::shared_ptr<NPC> Npc)
{

}



void NPC_ALIVE::NPC_Action(std::shared_ptr<NPC> Npc)
{
	//공격을 했음..
	std::shared_ptr<BULLET> Bullet;
	for (auto it : IocpConstructor::Object_Manager->OBJECT_MAP)
	{
		float dx = it.second->x;
		float dy = it.second->y;
		if (it.second->type == Object_USER)
		{
			std::shared_ptr<USER> User = std::static_pointer_cast<USER>(it.second);
			if ((sqrt((Npc->x - dx)*(Npc->x - dx) + (Npc->y - dy)*(Npc->y - dy)) < DISTANCE) && ((User->CurrentState == USER::state::ALIVE) || User->CurrentState == USER::state::STOP))
			{
				//총알을 어떻게 발사할건지 결정해줘야함...
				std::shared_ptr<BULLET> Bullet(new BULLET(Npc->x, Npc->y, 10.0f, 10.0f, Npc->ObjectId));
				IocpConstructor::Object_Manager->REGIST_BULLET(Bullet);

				TimerJob job, job2;
				job.exectime = GetTickCount() + AI_ATTACK_DELAY;
				job.func = std::bind(&NPC::NPC_DESICION, std::static_pointer_cast<NPC>(IocpConstructor::Object_Manager->FIND(Npc->ObjectId)));
				job2.exectime = GetTickCount() + NEXT_TICK;
				job2.func = std::bind(&BULLET::BULLET_MOVE, Bullet);
				{
					LOCKING(IocpConstructor::queueLock)
					IocpConstructor::jobs.push_back(job);
					IocpConstructor::jobs.push_back(job2);
				}
				PacketHandler::GetInstance()->S_STOP_HANDLER(IocpConstructor::Object_Manager->FIND(Npc->ObjectId));
				PacketHandler::GetInstance()->S_SHOOT_HANDLER(Bullet);
				return;
			}
		}
		//Npc->x = Npc->x + (Npc->velocity * 0.03f * sin(ai->direction * PI / 180));
		//Npc->y = Npc->y + (ai->velocity * 0.03f * cos(ai->direction * PI / 180));
	}
	//엔피씨의 거리계산!!!
	TimerJob job;
	job.exectime = GetTickCount() + AI_MOVE_DELAY;
	job.func = std::bind(&NPC::NPC_DESICION, std::static_pointer_cast<NPC>(IocpConstructor::Object_Manager->FIND(Npc->ObjectId)));
	{
		LOCKING(IocpConstructor::queueLock);
		IocpConstructor::jobs.push_back(job);
	}
	PacketHandler::GetInstance()->S_MOVE_HANDLER(IocpConstructor::Object_Manager->FIND(Npc->ObjectId));
	return;
}