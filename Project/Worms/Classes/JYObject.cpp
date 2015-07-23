#include "JYPlayer.h"
#include "BaseExecutor.h"
#include "MoveExecutor.h"
#include "JYObjectManager.h"
#include "CoordinateConverter.h"

JYObject::JYObject(cocos2d::CCNode* pCCObject) : 
	m_pCCObject(pCCObject), 
	m_pJYObjectParent(nullptr),
	m_nObjectType(JYOBJECT_TYPE::JY_OBJECT),
	m_nUID(-1),
	m_fVelocity(0.0f),
	m_fDirection(0.0f){
	memset(m_Executors, 0, sizeof(m_Executors));
	JYObjectManager::getInstance()->pushObject(this);
}

JYObject::~JYObject() {
	for (int i = 0; i < __MaxExecutor; ++i)
	{
		delete m_Executors[i];
	}
	JYObjectManager::getInstance()->popObject(this);
}

void JYObject::setExecutor(BaseExecutor* param) {
	int type = param->getEnum();
	m_Executors[type] = param;
}

void JYObject::ReleaseExecutor(BaseExecutor* param) {
	int type = param->getEnum();
	m_Executors[type] = nullptr;
}

void JYObject::addChild(JYObject* const pJYObject){
	this->m_pJYChildrenVector.push_back(pJYObject);
	pJYObject->m_pJYObjectParent = this;
}

void JYObject::removeChildByName(const std::string& sName){
	auto iteratorTarget = this->getChildIteratorByName(sName);
	if (iteratorTarget == m_pJYChildrenVector.end()) {
		CCLOG("No child named %s", sName.c_str());
		return;
	}
	(*iteratorTarget)->m_pJYObjectParent = nullptr;
	this->m_pJYChildrenVector.erase(iteratorTarget);
}

std::vector<JYObject*>::iterator JYObject::getChildIteratorByName(const std::string& sName){
	for (auto it = m_pJYChildrenVector.begin(); it < m_pJYChildrenVector.end(); ++it){
		if ((*it)->getName() == sName){
			return it;
		}
	}
	return m_pJYChildrenVector.end();
}

JYObject* JYObject::getChildByName(const std::string& sName){
	auto iteratorChild = this->getChildIteratorByName(sName);
	if (iteratorChild == m_pJYChildrenVector.end()){
		CCLOG("No child named %s", sName.c_str());
		return nullptr;
	}
	return *iteratorChild;
}

void JYObject::tick(float fDeltaTime){
	for (int i = 0; i < _countof(m_Executors); ++i)
	{
		if (m_Executors[i] != nullptr)
		{
			m_Executors[i]->tick(fDeltaTime);
		}
	}
}

void JYObject::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){

}

void JYObject::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
}

void JYObject::onMouseMove(cocos2d::Event* pEvent){
}


void JYObject::onMouseDown(cocos2d::Event* pEvent){
}