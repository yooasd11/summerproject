#include "stdafx.h"
#include "NPCManager.h"


NPCManager::NPCManager()
{
	this->key = new Lock;
	this->NpcCount = 1;
}


NPCManager::~NPCManager()
{
	delete this->key;
}

void NPCManager::registNPC(std::shared_ptr<NPC> n)
{
	//생성되는 부분을 다시 처리해줘야함..
	LOCKING(this->key);
	this->mappingNPC[this->NpcCount++] = n;
	return;
}

std::shared_ptr<NPC> NPCManager::retNPC(int n)
{
	return this->mappingNPC[n];
}

void NPCManager::removeNPC(int n)
{
	LOCKING(this->key);
	this->mappingNPC.erase(n);
	return;
}