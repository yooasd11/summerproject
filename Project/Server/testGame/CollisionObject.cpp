#include "stdafx.h"
#include "CollisionObject.h"


CollisionObject::CollisionObject()
{
	this->key = new Lock;
}


CollisionObject::~CollisionObject()
{
	delete this->key;
}
