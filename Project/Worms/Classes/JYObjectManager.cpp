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

std::vector<JYObject*>::iterator JYObjectManager::findIteratorByUID(UINT& nUID){
	for (auto it = m_pJYObjectVector.begin(); it < m_pJYObjectVector.end(); ++it){
		if ((*it)->getUID() == nUID) {
			return it;
		}
	}
	return m_pJYObjectVector.end();

}


void JYObjectManager::pushObject(JYObject* const pObject){
	if (pObject == nullptr) return;
	if (findIteratorByObject(pObject) != m_pJYObjectVector.end())
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

JYObject* JYObjectManager::findObjectByUID(UINT& nUID){
	std::vector<JYObject*>::iterator retIt = findIteratorByUID(nUID);
	if (retIt == m_pJYObjectVector.end()) return nullptr;
	return *retIt;
}