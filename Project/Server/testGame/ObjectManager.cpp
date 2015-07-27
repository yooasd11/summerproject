#include "stdafx.h"
#include "ObjectManager.h"


ObjectManager::ObjectManager()
{
	this->key = new Lock;
	this->ObjectCount = 1;
}


ObjectManager::~ObjectManager()
{
	delete this->key;
}

void ObjectManager::registObject(std::shared_ptr<CollisionObject> ob)
{
	LOCKING(this->key);
	this->mappingObject[this->ObjectCount++] = ob;
	return;
}

std::shared_ptr<CollisionObject>  ObjectManager::retObject(int ob)
{
	return this->mappingObject[ob];
}

void ObjectManager::removeObject(int ob)
{
	LOCKING(this->key);
	this->mappingObject.erase(ob);
	return;
}