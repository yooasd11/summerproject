#include "stdafx.h"

#include "NPC_STATE.h"

#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)


float Calculate(float, float, float, float);
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
	//공격을 했음..락을 어떻게 걸지..
	//LOCKING(&this->key);
	std::shared_ptr<BULLET> Bullet;
	std::map<int, std::shared_ptr<OBJECT>> Instance_map = IocpConstructor::Object_Manager->OBJECT_MAP;
	//for (auto it : IocpConstructor::Object_Manager->OBJECT_MAP)
	for (auto it : Instance_map)
	{
		float dx = it.second->x;
		float dy = it.second->y;
		if (it.second->type == Object_USER)
		{
			std::shared_ptr<USER> User = std::static_pointer_cast<USER>(it.second);
			if ((sqrt((Npc->x - dx)*(Npc->x - dx) + (Npc->y - dy)*(Npc->y - dy)) < DISTANCE) && ((User->CurrentState == USER::state::ALIVE) || User->CurrentState == USER::state::STOP))
			{
				printf("적발견\n");
				//총알을 어떻게 발사할건지 결정해줘야함...
				float bullet_vy = Calculate(Npc->x, Npc->y, dx, dy);
				float bullet_vx = BULLET_VX_DIRECTION;
				if (Npc->x > dx) bullet_vx *= -1;

				std::shared_ptr<BULLET> Bullet(new BULLET(Npc->x, Npc->y, bullet_vx, bullet_vy / BULLET_VELOCITY , Npc->ObjectId));
				IocpConstructor::Object_Manager->REGIST_BULLET(Bullet);

				TimerJob job, job2;
				job.exectime = GetTickCount() + AI_ATTACK_DELAY;
				job.func = std::bind(&NPC::NPC_DESICION, std::static_pointer_cast<NPC>(IocpConstructor::Object_Manager->FIND(Npc->ObjectId)));
				job2.exectime = GetTickCount() + NEXT_TICK;
				job2.func = std::bind(&BULLET::BULLET_MOVE, Bullet);
				{
					LOCKING(&IocpConstructor::queueLock)
					IocpConstructor::jobs.push_back(job);
					IocpConstructor::jobs.push_back(job2);
				}
				PacketHandler::GetInstance()->S_STOP_HANDLER(IocpConstructor::Object_Manager->FIND(Npc->ObjectId));
				PacketHandler::GetInstance()->S_SHOOT_HANDLER(Bullet);
				return;
			}
		}
	}
	//엔피씨의 거리계산!!!
	Npc->x += Npc->vx * 0.03f;
	Npc->y += Npc->vy * 0.03f;

	TimerJob job;
	job.exectime = GetTickCount() + AI_MOVE_DELAY;
	job.func = std::bind(&NPC::NPC_DESICION, std::static_pointer_cast<NPC>(IocpConstructor::Object_Manager->FIND(Npc->ObjectId)));
	{
		LOCKING(&IocpConstructor::queueLock);
		IocpConstructor::jobs.push_back(job);
	}
	PacketHandler::GetInstance()->S_MOVE_HANDLER(IocpConstructor::Object_Manager->FIND(Npc->ObjectId));
	return;
}

float Calculate(float n_x, float n_y, float u_x, float u_y)
{
	float time = (n_x - u_x) / BULLET_VELOCITY * BULLET_VX_DIRECTION;
	float vy = (n_y - u_y) / time - GRAVITY * time / 2;
	return vy;
}