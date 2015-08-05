#include "Inputhandler.h"
#include "JYPlayer.h"
#include "JYObjectManager.h"
#include "MyScene.h"

void InputHandler::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	bIsKeyPressed[(int)keyCode] = true;
	MyScene* pMyScene = GET_MYSCENE;
	JYPlayer* pJYPlayer = pMyScene->pJYPlayerDragon;
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onKeyPressed(keyCode, pEvent);
}

void InputHandler::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
 	bIsKeyPressed[(int)keyCode] = false;
	MyScene* pMyScene = GET_MYSCENE;
	JYPlayer* pJYPlayer = pMyScene->pJYPlayerDragon;
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onKeyReleased(keyCode, pEvent);
}

void InputHandler::onMouseMove(cocos2d::Event* pEvent){
	MyScene* pMyScene = GET_MYSCENE;
	JYPlayer* pJYPlayer = pMyScene->pJYPlayerDragon;
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onMouseMove(pEvent);

	cocos2d::CCNode* pBackground = pMyScene->getChildByName("Background");

}

void InputHandler::onMouseDown(cocos2d::Event* pEvent){
	MyScene* pMyScene = GET_MYSCENE;
	JYPlayer* pJYPlayer = pMyScene->pJYPlayerDragon;
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onMouseDown(pEvent);
}

void InputHandler::onMouseUp(cocos2d::Event* pEvent){
	MyScene* pMyScene = GET_MYSCENE;
	JYPlayer* pJYPlayer = pMyScene->pJYPlayerDragon;
	if (pJYPlayer == nullptr) return;
	pJYPlayer->onMouseUp(pEvent);
}

void InputHandler::onMouseScroll(cocos2d::Event* pEvent){
	cocos2d::EventMouse* e = (cocos2d::EventMouse*)pEvent;
	cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	cocos2d::CCParallaxNode* pBackground = (cocos2d::CCParallaxNode*)pTmap->getParent();
	float fScrollY = e->getScrollY() * -0.1f;
	float fCurrentScale = pBackground->getScale();
	float fNextScale = fCurrentScale + fScrollY;
	CCLOG("Current map scale : %.2f, next map scale : %.2f", fCurrentScale, fNextScale);
	pBackground->setScale(fNextScale);
}
