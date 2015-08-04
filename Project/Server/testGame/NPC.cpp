#include "stdafx.h"
#include "NPC.h"


NPC::NPC()
{
	this->key = new Lock();
	this->npc_alive = new NPC_ALIVE;
	this->npc_dead = new NPC_DEAD;
	this->current_state = npc_alive;
	this->type = Object_NPC;
}

NPC::NPC(float _x, float _y, float _vx, float _vy)
{
	this->key = new Lock();
	this->npc_alive = new NPC_ALIVE;
	this->npc_dead = new NPC_DEAD;
	this->current_state = npc_alive;
	this->type = Object_NPC;
	this->x = _x;
	this->y = _y;
	this->vx = _vx;
	this->vy = _vy;
	this->hp = 100;
}

NPC::~NPC()
{
	delete this->key;
	delete this->npc_alive;
	delete this->npc_dead;
}


void NPC::NPC_SET_HP(int _hp)
{
	LOCKING(this->key);
	this->hp = _hp;
	return;
}

void NPC::NPC_STATE_CHANGE(int _state)
{
	LOCKING(this->key);
	if (this->current_state == this->npc_dead) return;
	if (_state == NPC_STATUS_ALIVE) this->current_state = this->npc_alive;
	else if (_state == NPC_STATUS_DEAD) this->current_state = this->npc_dead;
	return;
}

void NPC::NPC_INIT()
{
	TimerJob job;
	job.exectime = GetTickCount() + NEXT_TICK;
	job.func = std::bind(&NPC::NPC_DESICION, std::static_pointer_cast<NPC>(IocpConstructor::Object_Manager->FIND(this->ObjectId)));
	{
		LOCKING(IocpConstructor::queueLock);
		IocpConstructor::jobs.push_back(job);
	}
	return;
}

void NPC::NPC_DESICION()
{
	//LOCKING(this->key);
	this->current_state->NPC_Action(std::static_pointer_cast<NPC>(IocpConstructor::Object_Manager->FIND(this->ObjectId)));
	return;
}

void NPC::NPC_DIRECTION_CHANGE()
{
	LOCKING(this->key);
	this->vx *= -1;
	return;
}