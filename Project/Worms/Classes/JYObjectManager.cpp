#include "JYObjectManager.h"

std::vector<JYObject*>::iterator JYObjectManager::findIteratorByName(const std::string& sName){
	for (auto it = m_pJYObjectVector.begin(); it < m_pJYObjectVector.end(); ++it){
		if ((*it)->getName() == sName) {
			return it;
		}
	}
	return m_pJYObjectVector.end();
}
std::vector<JYObject*>::iterator JYObjectManager::findIteratorByObject(JYObject* const pObject){
	for (auto it = m_pJYObjectVector.begin(); it < m_pJYObjectVector.end(); ++it){
		if ((*it) == pObject) {
			return it;
		}
	}
	return m_pJYObjectVector.end();

}

std::vector<JYObject*>::iterator JYObjectManager::findIteratorByUID(const UINT& nUID){
	for (auto it = m_pJYObjectVector.begin(); it < m_pJYObjectVector.end(); ++it){
		if ((*it)->getUID() == nUID) {
			return it;
		}
	}
	return m_pJYObjectVector.end();

}


void JYObjectManager::pushObject(JYObject* const pObject){
	if (pObject == nullptr) return;
	if (findIteratorByObject(pObject) == m_pJYObjectVector.end())
		m_pJYObjectVector.push_back(pObject);
}

void JYObjectManager::popObject(JYObject* const pObject){
	if (pObject == nullptr) return;
	std::vector<JYObject*>::iterator retIt = findIteratorByObject(pObject);
	if (retIt != m_pJYObjectVector.end())
		m_pJYObjectVector.erase(retIt);
}

JYObject* JYObjectManager::findObjectByName(const std::string& sName){
	std::vector<JYObject*>::iterator retIt = findIteratorByName(sName);
	if (retIt == m_pJYObjectVector.end()) return nullptr;
	return *retIt;
}

JYObject* JYObjectManager::findObjectByUID(const UINT& nUID){
	std::vector<JYObject*>::iterator retIt = findIteratorByUID(nUID);
	if (retIt == m_pJYObjectVector.end()) return nullptr;
	return *retIt;
}

void JYObjectManager::removeObject(JYObject* pJYObject, bool bCleanUp){
	std::vector<JYObject*>::iterator retIt = findIteratorByObject(pJYObject);
	if (retIt == m_pJYObjectVector.end()) return;
	this->popObject(pJYObject);
	JYObject* pJYParent = pJYObject->getParent();
	if (pJYParent != nullptr){
		pJYParent->removeChildByJYObject(pJYObject);
	}
	delete pJYObject;
	if (bCleanUp == true){
		//자식들 싹다 지우자
	}
}

void JYObjectManager::tick(float fDeltaTime){
	for (int i = 0; i < m_pJYObjectVector.size(); ++i){
		m_pJYObjectVector[i]->tick(fDeltaTime);
	}
}

void JYObjectManager::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	for (int i = 0; i < m_pJYObjectVector.size(); ++i){
		m_pJYObjectVector[i]->onKeyPressed(keyCode, pEvent);
	}
}

void JYObjectManager::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	for (int i = 0; i < m_pJYObjectVector.size(); ++i){
		m_pJYObjectVector[i]->onKeyReleased(keyCode, pEvent);
	}
}

void JYObjectManager::onMouseMove(cocos2d::Event* pEvent){
	for (int i = 0; i < m_pJYObjectVector.size(); ++i){
		m_pJYObjectVector[i]->onMouseMove(pEvent);
	}
}