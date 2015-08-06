#include "stdafx.h"
#include "USER.h"



USER::USER()
{
	this->hp = 100;
	this->CurrentState = USER::state::ALIVE;
}
USER::USER(int TYPE, float x, float y, int _socket)
{

	this->type = TYPE;
	this->ObjectId = ObjectManager::OBJECTCOUNT;
	this->x = x;
	this->y = y;
	this->hp = 100;
	this->CurrentState = USER::state::ALIVE;
	this->socket = _socket;
	this->vx = 0.0f;
	this->vy = 0.0f;
	this->ax = 0.0f;
	this->ay = GRAVITY;
}

USER::~USER()
{

}

void USER::SetAccelate(float _ax, float _ay)
{
	LOCKING(&this->key);
	this->ax = _ax;
	this->ay = _ay;
}
void USER::SetVelocity(float _vx, float _vy)
{
	LOCKING(&this->key);
	this->vx = _vx;
	this->vy = _vy;
}

void USER::ChangeState(int _state)
{
	LOCKING(&this->key);
	if (this->CurrentState != USER::state::DISCONNECT){
		this->CurrentState = (USER::state)_state;
	}
	return;
}

void USER::USER_RESPAWN()
{
	LOCKING(&this->key);
	this->CurrentState = USER::state::ALIVE;
	this->hp = 100;
	this->vx = 0.0f;
	this->vy = 0.0f;
	TimerJob job;
	job.exectime = GetTickCount();
	job.func = std::bind(&USER::USER_MOVE, std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(this->ObjectId)));
	{
		LOCKING(&IocpConstructor::queueLock);
		IocpConstructor::jobs.push_back(job);
	}
	PacketHandler::GetInstance()->S_RESPAWN_HANDLER(IocpConstructor::Object_Manager->FIND(this->ObjectId));
	return;
}

void USER::USER_MOVE()
{

	TimerJob User_Move_Job;
	LOCKING(&this->key);
	if (this->CurrentState == USER::state::ALIVE)
	{
		float t_vx = this->vx + this->ax * 0.03f;
		float dx = this->x + t_vx * 0.03f;

		float t_vy = this->vy + this->ay * 0.03f;
		float dy = this->y + t_vy * 0.03f;
		

		if (dx > WIDTH || dx < 0.0f || dy > HEIGHT || dy < LAND){
			this->vy = 0.0f;
			this->ay = 0.0f;
			if (dy > HEIGHT)
				this->ay = GRAVITY;
		}
	/*	if (this->vy == 0.0f){
			this->ay = 0;
			this->vy = 0;
			this->vx = t_vx;
			this->x = dx;
		}*/

		/*if (dy < LAND)
		{
			this->vx = t_vx;
			this->vy = 0;
			this->x = this->x;
			this->y = LAND + 1;
		}
		else if (dy > HEIGHT)
		{
			this->vx = t_vx;
			this->vy = 0;
			this->x = dx;
			this->y = HEIGHT - 10;

		}*/
		else if (!(dx > WIDTH || dx < 0.0f || dy > HEIGHT || dy < LAND))
		{
			this->vx = t_vx;
			this->vy = t_vy;
			this->x = dx;
			this->y = dy;
		}
		PacketHandler::GetInstance()->S_MOVE_HANDLER(IocpConstructor::Object_Manager->FIND(this->ObjectId));
		User_Move_Job.func = std::bind(&USER::USER_MOVE, std::static_pointer_cast<USER>(IocpConstructor::Object_Manager->FIND(this->ObjectId)));
		User_Move_Job.exectime = GetTickCount() + 30;
		{
			LOCKING(&IocpConstructor::queueLock)
			IocpConstructor::jobs.push_back(User_Move_Job);
		}
	}
	return;
}

void USER::USER_SET_HP(int _hp)
{
	LOCKING(&this->key);
	this->hp = _hp;
	return;
}


void USER::CALCULATE_DISTANCE()
{
	this->x = this->x + (this->vx * 0.03 );
	this->y = this->y + (this->vy * 0.03 );
	return;
}