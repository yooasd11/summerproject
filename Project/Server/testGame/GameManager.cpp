#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
	this->key = new Lock();
	this->bulletCount = 1;
}

GameManager::~GameManager()
{
	delete this->key;
}

void GameManager::registBullet(std::shared_ptr<bullet> b)
{
	LOCKING(this->key);
	this->mappingBullet[this->bulletCount++] = b;
	return;
}

std::shared_ptr<bullet> GameManager::retBullet(int _th){
	return this->mappingBullet[_th];
}

void GameManager::removeBullet(int th)
{
	LOCKING(this->key);
	this->mappingBullet.erase(th);
	return;
}