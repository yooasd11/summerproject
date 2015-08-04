#include "stdafx.h"
#include "ObjectManager.h"

int ObjectManager::OBJECTCOUNT;

ObjectManager::ObjectManager()
{
	this->OBJECTCOUNT = 1;
	this->key = new Lock();
}


ObjectManager::~ObjectManager()
{
	delete this->key;
}


void ObjectManager::REGIST(int TYPE, int info)
{
	LOCKING(this->key);
	if (TYPE == Object_USER)
	{
		//user일경우 소켓번호를 info..
		this->OBJECT_MAP[this->OBJECTCOUNT++] = std::shared_ptr<USER>(new USER(TYPE, 100.0f, 100.0f, info));
	}
	else if (TYPE == Object_NPC)
	{
		this->OBJECT_MAP[this->OBJECTCOUNT++] = std::shared_ptr<NPC>(new NPC(100.0f, 100.0f, 100.0f, 100.0f));
	}
	else return;
}

std::shared_ptr<OBJECT> ObjectManager::FIND(int _id)
{
	LOCKING(this->key);
	if (this->OBJECT_MAP.count(_id) != 0)
		return this->OBJECT_MAP[_id];
	return NULL;
}

void ObjectManager::REMOVE(int _id)
{
	LOCKING(this->key);
	if (this->OBJECT_MAP.count(_id) != 0){
		if (this->OBJECT_MAP[_id]->type == Object_USER){
			printf("%d user disconnected..\n", _id);
		}
		this->OBJECT_MAP.erase(_id);
	}
	return;
}

int ObjectManager::FIND_USER(int _socket)
{
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
	LOCKING(this->key);
	Bullet->ObjectId = OBJECTCOUNT;
	this->OBJECT_MAP[this->OBJECTCOUNT++] = Bullet;
	return;
}

void ObjectManager::REGIST_NPC(std::shared_ptr<NPC> Npc)
{
	LOCKING(this->key);
	Npc->ObjectId = OBJECTCOUNT;
	this->OBJECT_MAP[this->OBJECTCOUNT++] = Npc;
	return;
}