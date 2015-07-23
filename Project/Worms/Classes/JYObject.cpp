#include "JYPlayer.h"
#include "BaseExecuter.h"
#include "MoveExecuter.h"
#include "JYObjectManager.h"
#include "CoordinateConverter.h"

JYObject::JYObject(cocos2d::CCNode* pCCObject) : 
	m_pCCObject(pCCObject), 
	m_pJYObjectParent(nullptr),
	m_nObjectType(JYOBJECT_TYPE::JY_OBJECT),
	m_nUID(-1),
	m_fVelocity(0.0f),
	m_fDirection(0.0f){
	memset(m_Executers, 0, sizeof(m_Executers));
	JYObjectManager::getInstance()->pushObject(this);
}

JYObject::~JYObject() {
	for (int i = 0; i < __Executer::__Executer_END; ++i)
	{
		delete m_Executers[i];
	}
	this->getParent()->removeChildByJYObject(this);
	JYObjectManager::getInstance()->popObject(this);
	this->removeChildrenRecursive(this);
}

void JYObject::removeChildrenRecursive(JYObject* here){
	if (here == nullptr) return;
	for (int i = 0; i < this->m_pJYChildrenVector.size(); ++i){
		removeChildrenRecursive(this->m_pJYChildrenVector[i]);
	}
	delete this;
}

void JYObject::setExecuter(BaseExecuter* param) {
	int type = param->getEnum();
	m_Executers[type] = param;
}

void JYObject::ReleaseExecuter(BaseExecuter* param) {
	int type = param->getEnum();
	m_Executers[type] = nullptr;
}

void JYObject::addChild(JYObject* const pJYObject){
	this->m_pJYChildrenVector.push_back(pJYObject);
	pJYObject->m_pJYObjectParent = this;
}

void JYObject::removeChildByJYObject(JYObject* pJYObject){
	for (int i = 0; i < this->m_pJYChildrenVector.size(); ++i){
		if (this->m_pJYChildrenVector[i] == pJYObject){
			this->m_pJYChildrenVector.erase(this->m_pJYChildrenVector.begin() + i);
			return;
		}
	}
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
	for (int i = 0; i < _countof(m_Executers); ++i){
		if (m_Executers[i] != nullptr){
			m_Executers[i]->tick(fDeltaTime);
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