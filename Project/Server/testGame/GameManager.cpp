#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
	this->key = new Lock();
}

GameManager::~GameManager()
{
	delete this->key;
}

void GameManager::registBullet(std::shared_ptr<bullet> b)
{
	this->mappingBullet[IocpConstructor::ObjectCount++] = b;
	return;
}

std::shared_ptr<bullet> GameManager::retBullet(int id){
	return this->mappingBullet[id];
}

void GameManager::removeBullet(int id)
{
	LOCKING(this->key);
	this->mappingBullet.erase(id);
	return;
}