#include "BaseExecuter.h"
#include "JYObject.h"

BaseExecuter::~BaseExecuter(){
	if (m_pOwner != nullptr)
		m_pOwner->ReleaseExecuter(this);

}

void BaseExecuter::regist(){ 
	m_pOwner->setExecuter(this); 
}

void BaseExecuter::tick(float fDeltaTime){
}
