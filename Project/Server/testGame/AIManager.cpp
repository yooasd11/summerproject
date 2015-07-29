#include "stdafx.h"
#include "AIManager.h"


AIManager::AIManager()
{
	this->key = new Lock;
	this->NpcCount = 1;
}


AIManager::~AIManager()
{
	delete this->key;
}

void AIManager::registAI(std::shared_ptr<AI> n)
{
	//�����Ǵ� �κ��� �ٽ� ó���������..
	LOCKING(this->key);
	this->mappingAI[this->NpcCount++] = n;
	return;
}

std::shared_ptr<AI> AIManager::retAI(int n)
{
	return this->mappingAI[n];
}

void AIManager::removeAI(int n)
{
	LOCKING(this->key);
	this->mappingAI.erase(n);
	return;
}