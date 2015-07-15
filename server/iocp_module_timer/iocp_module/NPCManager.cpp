#include "stdafx.h"
#include "NPCManager.h"


NPCManager::NPCManager()
{
	this->ioInfo = new IoData;
	this->NPCcount = 0;
}


NPCManager::~NPCManager()
{
	
}


////임시로만든것...
void NPCManager::tempFunction(){

	for (int i = 0; i < 10; i++){
		this->_NPC[i].setNPC(std::to_string(i), i, i, i);
	}
}

void NPCManager::StartNPC(HANDLE hPort){

	
	return;
}