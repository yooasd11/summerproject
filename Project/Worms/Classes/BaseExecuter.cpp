#include "BaseExecuter.h"
#include "JYObject.h"

BaseExecuter::~BaseExecuter(){
	m_pOwner = nullptr;
}

void BaseExecuter::regist(){ 
	m_pOwner->setExecuter(this); 
}

void BaseExecuter::tick(float fDeltaTime){
	this->m_JYLocalTimer.tick(fDeltaTime);
}
