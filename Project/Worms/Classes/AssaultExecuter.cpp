#pragma once
#include "AssaultExecuter.h"
#include "MyScene.h"
#include "JYArm.h"
#include "JYPlayer.h"
#include "CoordinateConverter.h"
#include "InGamePacket.pb.h"
#include "Packet.h"
#include "ConnectionManager.h"

bool AssaultExecuter::coolTimeAlarmFunc(){
	this->m_bIsCooling = false;
	this->m_pCCSpriteCoolTimer->setVisible(false);
	return true;
}


void AssaultExecuter::createCoolTimer(){
	cocos2d::CCNode* pCCOwner = this->getOwner()->getCCObject();
	this->m_pCCSpriteCoolTimer = cocos2d::CCSprite::create();
	this->m_pCCSpriteCoolTimer->setScale(0.2f);
	pCCOwner->addChild(this->m_pCCSpriteCoolTimer);
	this->m_pCCSpriteCoolTimer->setPosition(cocos2d::ccp(pCCOwner->getContentSize().width/2, 0.0f));

	std::function<bool()> coolTimeAlarm = std::bind(&AssaultExecuter::coolTimeAlarmFunc, this);
	JYLocalTimer coolTimer(3.0f, coolTimeAlarm);
	coolTimer.setAlarmFunc(coolTimeAlarm);
	this->m_vecTimers.push_back(coolTimer);
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

	this->executeCoolTimer();
}


void AssaultExecuter::executeCoolTimer(){
	//lock
	this->m_bIsCooling = true;
	
	cocos2d::CCNode* pCCOwner = this->getOwner()->getCCObject();
	this->m_pCCSpriteCoolTimer->setVisible(true);
	cocos2d::CCAnimation* pCCCoolTimeAnimation = cocos2d::CCAnimation::create();
	pCCCoolTimeAnimation->setDelayPerUnit(1.0f);
	pCCCoolTimeAnimation->addSpriteFrameWithFileName("TimerPics/2.png");
	pCCCoolTimeAnimation->addSpriteFrameWithFileName("TimerPics/1.png");
	pCCCoolTimeAnimation->addSpriteFrameWithFileName("TimerPics/0.png");
	cocos2d::CCAnimate* pCCAnimateCoolTimer = cocos2d::Animate::create(pCCCoolTimeAnimation);
	this->m_pCCSpriteCoolTimer->runAction(pCCAnimateCoolTimer);

}

void AssaultExecuter::tick(float fDeltaTime){
	for (int i = 0; i < this->m_vecTimers.size(); ++i)
	{
		m_vecTimers[i].addTime(fDeltaTime);
		if (m_vecTimers[i].isDeadlined() == true)
		{
			m_vecTimers[i].alarm();
		}
	}
}