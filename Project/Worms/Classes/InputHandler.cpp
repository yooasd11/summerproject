#include "Inputhandler.h"
#include "JYObject.h"
#include "JYObjectManager.h"
#include "MyScene.h"

void InputHandler::pushObject(JYObject* pObject){
	JYObjectManager::getInstance()->pushObject(pObject);
}

void InputHandler::popObject(JYObject* pObject){
	JYObjectManager::getInstance()->popObject(pObject);
}

void InputHandler::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	bIsKeyPressed[(int)keyCode] = true;
	MyScene* pMyScene = GET_MYSCENE;
	JYObject* pJYPlayer = JYObjectManager::getInstance()->findObjectByUID(pMyScene->nPlayerUID);
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onKeyPressed(keyCode, pEvent);
}

void InputHandler::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
 	bIsKeyPressed[(int)keyCode] = false;
	MyScene* pMyScene = GET_MYSCENE;
	JYObject* pJYPlayer = JYObjectManager::getInstance()->findObjectByUID(pMyScene->nPlayerUID);
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onKeyReleased(keyCode, pEvent);
}

void InputHandler::onMouseMove(cocos2d::Event* pEvent){
	MyScene* pMyScene = GET_MYSCENE;
	JYObject* pJYPlayer = JYObjectManager::getInstance()->findObjectByUID(pMyScene->nPlayerUID);
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onMouseMove(pEvent);
}

void InputHandler::onMouseDown(cocos2d::Event* pEvent){
	MyScene* pMyScene = GET_MYSCENE;
	JYObject* pJYPlayer = JYObjectManager::getInstance()->findObjectByUID(pMyScene->nPlayerUID);
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onMouseDown(pEvent);
}


void InputHandler::tick(float fDeltaTime){
	JYObjectManager::getInstance()->tick(fDeltaTime);
}

