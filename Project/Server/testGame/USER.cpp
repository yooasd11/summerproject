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
}

USER::~USER()
{

}

void USER::SetVelocity(float _vx, float _vy)
{
	LOCKING(&this->key);
	
	this->vx = _vx * USER_VELOCITY;
	this->vy = _vy * USER_VELOCITY;
}

void USER::ChangeState(int _state)
{
	LOCKING(&this->key);
	if (this->CurrentState != USER::state::DISCONNECT){
		this->CurrentState = (USER::state)_state;
	}
	return;
}

void USER::USER_MOVE()
{
	TimerJob User_Move_Job;
	LOCKING(&this->key);
	if (this->CurrentState == USER::state::ALIVE)
	{
		float dx = this->x + (this->vx * 0.03f);
		float dy = this->y + (this->vy * 0.03f);
		if (!(dx > WIDTH || dx < 0.0f || dy > HEIGHT || dy < LAND))
		{
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