#pragma once
#include "AssaultExecuter.h"
#include "MyScene.h"
#include "JYArm.h"
#include "JYPlayer.h"
#include "CoordinateConverter.h"
#include "InGamePacket.pb.h"
#include "Packet.h"
#include "ConnectionManager.h"


void AssaultExecuter::createCoolTimer(){
	cocos2d::CCNode* pCCOwner = this->getOwner()->getCCObject();
	cocos2d::CCSprite* pCCSpriteTimer = cocos2d::CCSprite::create("TimerPics/timer.png");
	pCCSpriteTimer->setScale(0.1f);
	this->m_pCCCoolTimer = cocos2d::CCProgressTimer::create(pCCSpriteTimer);
	this->m_pCCCoolTimer->setType(CCProgressTimer::Type::RADIAL);
	this->m_pCCCoolTimer->setName("CoolTimer");
	this->m_pCCCoolTimer->setScale(0.2f);
	pCCOwner->addChild(this->m_pCCCoolTimer);
	this->m_pCCCoolTimer->setPosition(cocos2d::ccp(pCCOwner->getContentSize().width / 2, 0.0f));
}

void AssaultExecuter::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
}

void AssaultExecuter::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){

}

void AssaultExecuter::onMouseMove(cocos2d::Event* pEvent){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;
	cocos2d::CCSprite* pFireAim = (cocos2d::CCSprite*)pCCOwner->getChildByName("Aim");
	if (pFireAim == nullptr) return;
	float degree = CoordinateConverter::getInstance()->getDegreeBetweenCCNodeAndMouse(pFireAim, pEvent);

	pFireAim->setRotation(degree);
}

void AssaultExecuter::onMouseDown(cocos2d::Event* pEvent){
	if (this->m_bIsCooling == true) return;

	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	float fDirection = CoordinateConverter::getInstance()->getDegreeBetweenCCNodeAndMouse(pCCOwner, pEvent);
	CCLOG("Fire direction : %.2f", fDirection);

	char sendBuf[PKTLENGTH];
	InGamePacket::C_Shoot c_shoot;

	c_shoot.set_uid(this->getOwner()->getUID());
	c_shoot.set_direction(fDirection);

	c_shoot.SerializeToArray(sendBuf, c_shoot.ByteSize());

	ConnectionManager::getInstance()->transmit(c_shoot.ByteSize(), PACKET_TYPE::PKT_C_SHOOT, sendBuf);
	WAITING_FOR(PACKET_TYPE::PKT_S_SHOOT);

	this->executeCoolTimer(3.0f);
}


void AssaultExecuter::executeCoolTimer(float fSec){
	//lock
	this->m_bIsCooling = true;
	
	cocos2d::CCNode* pCCOwner = this->getOwner()->getCCObject();
	this->m_pCCCoolTimer->setVisible(true);
	m_pCCCoolTimer->setPercentage(0.0f);

	this->m_JYLocalTimer.pushTimer(
		fSec,
		std::bind(&AssaultExecuter::coolTimeCallBackFunc, this),
		std::bind(&AssaultExecuter::coolTimeDeltaFunc, this, std::placeholders::_1)
		);
}

bool AssaultExecuter::coolTimeCallBackFunc(){
	this->m_bIsCooling = false;
	this->m_pCCCoolTimer->setVisible(false); 
	this->m_pCCCoolTimer->setPercentage(0.0f);
	return true;
}

bool AssaultExecuter::coolTimeDeltaFunc(const float& fDeltaTime){
	if (this->m_bIsCooling == true){
		float fNextPercentage = this->m_pCCCoolTimer->getPercentage() + 100 * fDeltaTime / 3.0f;
		this->m_pCCCoolTimer->setPercentage(fNextPercentage);
	}
	return true;
}


void AssaultExecuter::tick(float fDeltaTime){
	this->m_JYLocalTimer.tick(fDeltaTime);
}