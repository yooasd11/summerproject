#pragma once
#include "JYPlayer.h"
#include "MoveRequestExecuter.h"
#include "MoveExecuter.h"
#include "AssaultExecuter.h"
#include "AccelerationExecuter.h"
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
	new AccelerationExecuter(this);
	this->setObjectType(JYOBJECT_TYPE::JY_PLAYER);
}

void JYPlayer::createRespawnTimer(){
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)this->getCCObject();
	cocos2d::CCSprite* pCCSpriteDead = cocos2d::CCSprite::create("dead.jpg");
	this->m_pCCRespawnTimer = cocos2d::CCProgressTimer::create(pCCSpriteDead);
	this->m_pCCRespawnTimer->setType(CCProgressTimer::Type::RADIAL);
	this->m_pCCRespawnTimer->setName("RespawnTimer");
	this->m_pCCRespawnTimer->setPercentage(100.0f);
	this->m_pCCRespawnTimer->setScale(0.4f);
	this->m_pCCRespawnTimer->setPosition(cocos2d::ccp(pCCOwner->getContentSize().width / 2, 50.0f));
	this->m_pCCRespawnTimer->setVisible(true);
	pCCOwner->addChild(this->m_pCCRespawnTimer);
}

void JYPlayer::executeRespawnTimer(const float& fSec){
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)this->getCCObject();
	pCCOwner->stopAllActions();
	//pCCOwner->setVisible(false);
	this->releaseAllExecuters();

	this->m_pCCRespawnTimer->setVisible(true);
	this->getLocalTimer()->pushTimer(
		fSec,
		std::bind(&JYPlayer::respawnCallBackFunc, this),
		std::bind(&JYPlayer::respawnDeltaTimeFunc, this, std::placeholders::_1)
		);
}

bool JYPlayer::respawnDeltaTimeFunc(const float& fDeltaTime){
	float fNextPercentage = this->m_pCCRespawnTimer->getPercentage() - 100 * fDeltaTime / 5.0f;
	this->m_pCCRespawnTimer->setPercentage(fNextPercentage);
	return true;
}

bool JYPlayer::respawnCallBackFunc(){
	return true;
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
		this->setVelocity(0.0f, 0.0f);
		this->createRespawnTimer();
		this->executeRespawnTimer(5.0f);
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

void JYPlayer::onMouseUp(cocos2d::Event* pEvent){
	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onMouseUp(pEvent);
	}
}

void JYPlayer::onMouseDown(cocos2d::Event* pEvent){
	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onMouseDown(pEvent);
	}
}
