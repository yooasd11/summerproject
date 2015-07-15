#include "JYObject.h"
#include "BaseExecutor.h"
#include "MoveExecutor.h"
#include "InputHandler.h"

JYObject::JYObject(cocos2d::CCNode* pCCObject) : m_pCCObject(pCCObject) {
	memset(m_Executors, 0, sizeof(m_Executors));
	new MoveExecutor(this);
	InputHandler::getInstance()->pushObject(this);
}

JYObject::~JYObject() {
	for (int i = 0; i < __MaxExecutor; ++i)
	{
		delete m_Executors[i];
	}
	InputHandler::getInstance()->popObject(this);
}

void JYObject::setExecutor(BaseExecutor* param) {
	int type = param->getEnum();
	m_Executors[type] = param;
}

void JYObject::ReleaseExecutor(BaseExecutor* param) {
	int type = param->getEnum();
	m_Executors[type] = nullptr;
}

void JYObject::tick(float fDeltaTime){
	this->getExecutor(__MoveExecutor)->tick(fDeltaTime);
}