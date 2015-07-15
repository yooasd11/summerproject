#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
	this->State = WAITING;
	this->manageNPC = new NPCManager;
	this->manageNPC->tempFunction();
}


GameManager::~GameManager()
{
	delete this->manageNPC;
}


int GameManager::ChangeState(int s){
	this->State = s;
	return this->State;
}

//������ �����ϸ� ���ǽø� �����ϰ� ��������..
void GameManager::StartGame(){
	this->manageNPC->StartNPC(this->hPort);
	return;

}

void GameManager::setPort (HANDLE port){
	this->hPort = port;
	return;
}