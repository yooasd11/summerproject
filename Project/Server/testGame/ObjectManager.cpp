#include "stdafx.h"
#include "ObjectManager.h"

int ObjectManager::OBJECTCOUNT;

ObjectManager::ObjectManager()
{
	this->OBJECTCOUNT = 1;
	
}


ObjectManager::~ObjectManager()
{
	
}


void ObjectManager::REGIST(int TYPE, int info)
{
	//LOCKING(&this->key);
	LOCKING(&IocpConstructor::ObjectKey);
	if (TYPE == Object_USER)
	{
		//user일경우 소켓번호를 info..
		this->OBJECT_MAP[this->OBJECTCOUNT++] = std::shared_ptr<USER>(new USER(TYPE, 100.0f, 100.0f, info));
	}
	else return;
}

std::shared_ptr<OBJECT> ObjectManager::FIND(int _id)
{
	//LOCKING(&this->key);
	LOCKING(&IocpConstructor::ObjectKey);
	if (this->OBJECT_MAP.count(_id) != 0)
		return this->OBJECT_MAP[_id];
	return NULL;
}

void ObjectManager::REMOVE(int _id)
{
	LOCKING(&IocpConstructor::ObjectKey);
	if (this->OBJECT_MAP.count(_id) != 0){
		if (this->OBJECT_MAP[_id]->type == Object_USER){
			printf("%d user disconnected..\n", _id);
		}
		this->OBJECT_MAP.erase(_id);
	}
	return;
	/*LOCKING(&IocpConstructor::ObjectKey);
	if (this->OBJECT_MAP.count(_id) != 0){
		TimerJob job;
		job.exectime = GetTickCount() + 30;
		job.func = std::bind(&ObjectManager::REMOVE_JOB, this, _id);
		{
			LOCKING(&IocpConstructor::queueLock);
			IocpConstructor::jobs.push_back(job);
		}
	}
	return;*/
}


void ObjectManager::REMOVE_JOB(int _id)
{
	LOCKING(&IocpConstructor::ObjectKey);
	if (this->OBJECT_MAP[_id] != 0){
		if (this->OBJECT_MAP[_id].unique())
		{
			if (this->OBJECT_MAP[_id]->type == Object_USER)
				printf("%d user disconnected..\n", _id);
			this->OBJECT_MAP.erase(_id);
		}
		else{
			TimerJob job;
			job.exectime = GetTickCount() + 30;
			job.func = std::bind(&ObjectManager::REMOVE_JOB, this, _id);
			{
				LOCKING(&IocpConstructor::queueLock);
				IocpConstructor::jobs.push_back(job);
			}
		}
	}
	return;
}

int ObjectManager::FIND_USER(int _socket)
{
	LOCKING(&IocpConstructor::ObjectKey);
	for (auto user : this->OBJECT_MAP)
	{
		if (user.second->type == Object_USER)
		{
			std::shared_ptr<USER> tempUser = std::static_pointer_cast<USER>(user.second);
			if (tempUser->socket == _socket) return tempUser->ObjectId;
		}
	}
	return -1;
}

void ObjectManager::REGIST_BULLET(std::shared_ptr<BULLET> Bullet)
{
	//LOCKING(&this->key);
	LOCKING(&IocpConstructor::ObjectKey);
	Bullet->ObjectId = OBJECTCOUNT;
	this->OBJECT_MAP[this->OBJECTCOUNT++] = Bullet;
	return;
}

void ObjectManager::REGIST_NPC(std::shared_ptr<NPC> Npc)
{
	//LOCKING(&this->key);
	LOCKING(&IocpConstructor::ObjectKey);
	Npc->ObjectId = OBJECTCOUNT;
	this->OBJECT_MAP[this->OBJECTCOUNT++] = Npc;
	return;
}