#include "Inputhandler.h"
#include "JYObject.h"
#include "JYObjectManager.h"

void InputHandler::pushObject(JYObject* pObject){
	JYObjectManager::getInstance()->pushObject(pObject);
}

void InputHandler::popObject(JYObject* pObject){
	JYObjectManager::getInstance()->popObject(pObject);
}

void InputHandler::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	bIsKeyPressed[(int)keyCode] = true;
	std::vector<JYObject*>* JYObjectArray = &(JYObjectManager::getInstance()->m_pJYObjectVector);
	for (auto it = JYObjectArray->begin(); it < JYObjectArray->end(); ++it){
		(*it)->onKeyPressed(keyCode, pEvent);
	}
}

void InputHandler::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	bIsKeyPressed[(int)keyCode] = false;
	std::vector<JYObject*>* JYObjectArray = &(JYObjectManager::getInstance()->m_pJYObjectVector);
	for (auto it = JYObjectArray->begin(); it < JYObjectArray->end(); ++it){
		(*it)->onKeyReleased(keyCode, pEvent);
	}
}

void InputHandler::onMouseMove(cocos2d::Event* pEvent){
	std::vector<JYObject*>* JYObjectArray = &(JYObjectManager::getInstance()->m_pJYObjectVector);
	for (auto it = JYObjectArray->begin(); it < JYObjectArray->end(); ++it){
		(*it)->onMouseMove(pEvent);
	}
}

void InputHandler::tick(float fDeltaTime){
	std::vector<JYObject*>* JYObjectArray = &(JYObjectManager::getInstance()->m_pJYObjectVector);
	for (auto it = JYObjectArray->begin(); it < JYObjectArray->end(); ++it){
		(*it)->tick(fDeltaTime);
	}
}

