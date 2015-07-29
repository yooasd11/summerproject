#pragma once
#include "JYPlayer.h"
#include "MoveRequestExecuter.h"
#include "MoveExecuter.h"
#include "AssaultExecuter.h"
#include "CollisionExecuter.h"
#include "CoordinateConverter.h"
#include "Packet.h"
#include "Inputhandler.h"
#include "JYObjectManager.h"

JYPlayer::JYPlayer(cocos2d::CCNode* pCCObject) : JYObject(pCCObject){
	new MoveRequestExecuter(this);
	new MoveExecuter(this);
	new CollisionExecuter(this);
	new AssaultExecuter(this);
	this->setObjectType(JYOBJECT_TYPE::JY_PLAYER);
}

void JYPlayer::setHP(const UINT& nHP){
	this->m_nHP = nHP;
	cocos2d::CCProgressTimer* pHPBar = (cocos2d::CCProgressTimer*)this->getCCObject()->getChildByName("HPBar");
	if (pHPBar == nullptr){
		CCLOG("No HP bar found");
		return;
	}

	if (nHP > 0) {
		float fHPPercentage = nHP * 100.0f / 100.0f;
		cocos2d::ProgressTo* pProgressTo = cocos2d::ProgressTo::create(1.0f, fHPPercentage);
		pHPBar->runAction(pProgressTo);
	}
	else{
		cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)this->getCCObject();
		pCCOwner->stopAllActions();
		pCCOwner->removeAllChildren();
		pCCOwner->setTexture("HPBars/0.jpg");
		pCCOwner->setFlippedX(false);
		this->releaseAllExecuters();
	}
}

void JYPlayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	MoveRequestExecuter* pMoveRequestExecuter = (MoveRequestExecuter*)this->getExecuter(__Executer::__MoveRequestExecuter);
	if (pMoveRequestExecuter != nullptr){
		pMoveRequestExecuter->onKeyPressed(keyCode, pEvent);
	}

	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onKeyPressed(keyCode, pEvent);
	}
}

void JYPlayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	MoveRequestExecuter* pMoveRequestExecuter = (MoveRequestExecuter*)this->getExecuter(__Executer::__MoveRequestExecuter);
	if (pMoveRequestExecuter != nullptr){
		pMoveRequestExecuter->onKeyReleased(keyCode, pEvent);
	}

	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onKeyPressed(keyCode, pEvent);
	}
}

void JYPlayer::onMouseMove(cocos2d::Event* pEvent){
	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onMouseMove(pEvent);
	}
}

void JYPlayer::onMouseDown(cocos2d::Event* pEvent){
	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onMouseDown(pEvent);
	}
}
