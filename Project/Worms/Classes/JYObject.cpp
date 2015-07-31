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
	m_fVx(0.0f),
	m_fVy(0.0f){
	memset(m_Executers, 0, sizeof(m_Executers));
	JYObjectManager::getInstance()->pushObject(this);
}

JYObject::~JYObject() {
	//delete executers
	for (int i = 0; i < __Executer::__Executer_END; ++i){
		if (m_Executers[i] != nullptr)
			releaseExecuter((__Executer)i);
	}

	//pop this from parent
	if (this->getParent() != nullptr){
		this->getParent()->popChildByJYObject(this);
	}

	//delete CCObject 
	if (this->getCCObject() != nullptr && this->getCCObject()->getParent() != nullptr){
		this->getCCObject()->getParent()->removeChild(this->getCCObject());
	}

	//delete children
	for (int i = 0; i < m_pJYChildrenVector.size(); ++i){
		if (m_pJYChildrenVector[i] != nullptr)
			JYObjectManager::getInstance()->popObject(m_pJYChildrenVector[i]);
	}
}

void JYObject::setExecuter(BaseExecuter* param) {
	int type = param->getEnum();
	m_Executers[type] = param;
}

void JYObject::releaseExecuter(__Executer eType) {
	delete m_Executers[eType];
	m_Executers[eType] = nullptr;
}

void JYObject::releaseAllExecuters(){
	for (int i = 0; i < __Executer::__Executer_END; ++i){
		if (this->getExecuter((__Executer)i) != nullptr)
			this->releaseExecuter((__Executer)i);
	}
}

void JYObject::addChild(JYObject* const pJYObject){
	this->m_pJYChildrenVector.push_back(pJYObject);
	pJYObject->m_pJYObjectParent = this;
}

void JYObject::popChildByJYObject(JYObject* const pJYObject){
	auto iteratorTarget = this->getChildIteratorByObject(pJYObject);
	if (iteratorTarget == m_pJYChildrenVector.end()) {
		CCLOG("No Child");
		return;
	}
	(*iteratorTarget)->m_pJYObjectParent = nullptr;
	this->m_pJYChildrenVector.erase(iteratorTarget);
}

void JYObject::popChildByName(const std::string& sName){
	auto iteratorTarget = this->getChildIteratorByName(sName);
	if (iteratorTarget == m_pJYChildrenVector.end()) {
		CCLOG("No child named %s", sName.c_str());
		return;
	}
	(*iteratorTarget)->m_pJYObjectParent = nullptr;
	this->m_pJYChildrenVector.erase(iteratorTarget);
	this->getCCObject()->removeChildByName(sName);
}

std::vector<JYObject*>::iterator JYObject::getChildIteratorByTag(const UINT& nTag){
	for (auto it = m_pJYChildrenVector.begin(); it < m_pJYChildrenVector.end(); ++it){
		if ((*it)->getTag() == nTag){
			return it;
		}
	}
	return m_pJYChildrenVector.end();
}

std::vector<JYObject*>::iterator JYObject::getChildIteratorByObject(JYObject* const pJYObject){
	for (auto it = m_pJYChildrenVector.begin(); it < m_pJYChildrenVector.end(); ++it){
		if ((*it) == pJYObject){
			return it;
		}
	}
	return m_pJYChildrenVector.end();
}

std::vector<JYObject*>::iterator JYObject::getChildIteratorByName(const std::string& sName){
	for (auto it = m_pJYChildrenVector.begin(); it < m_pJYChildrenVector.end(); ++it){
		if ((*it)->getName() == sName){
			return it;
		}
	}
	return m_pJYChildrenVector.end();
}

JYObject* JYObject::getChildByTag(const UINT& nTag){
	auto iteratorChild = this->getChildIteratorByTag(nTag);
	if (iteratorChild == m_pJYChildrenVector.end()){
		CCLOG("No child taged %d", nTag);
		return nullptr;
	}
	return *iteratorChild;
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