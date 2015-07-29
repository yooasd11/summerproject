#pragma once
#include "AssaultExecuter.h"
#include "MyScene.h"
#include "JYArm.h"
#include "JYPlayer.h"
#include "CoordinateConverter.h"
#include "InGamePacket.pb.h"
#include "Packet.h"
#include "ConnectionManager.h"
#include "JYRealTimer.h"

void AssaultExecuter::createCoolTimer(){
	this->m_pCCSpriteCoolTimer = cocos2d::CCSprite::create("TimerPics/3.png");
	m_pCCSpriteCoolTimer->setVisible(false);
	cocos2d::CCAnimation* pCCCoolTimeAnimation = cocos2d::CCAnimation::create();
	pCCCoolTimeAnimation->setDelayPerUnit(1.0f);
	pCCCoolTimeAnimation->addSpriteFrameWithFileName("TimerPics/2.png");
	pCCCoolTimeAnimation->addSpriteFrameWithFileName("TimerPics/1.png");
	pCCCoolTimeAnimation->addSpriteFrameWithFileName("TimerPics/0.png");
	this->m_pCCAnimateCoolTimer = cocos2d::Animate::create(pCCCoolTimeAnimation);
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
	//this->executeCoolTimer();
}


void AssaultExecuter::executeCoolTimer(){
	//lock
	this->m_bIsCooling = true;
	
	cocos2d::CCNode* pCCOwner = this->getOwner()->getCCObject();
	this->m_pCCSpriteCoolTimer->setVisible(true);
	this->m_pCCSpriteCoolTimer->runAction(this->m_pCCAnimateCoolTimer);

	//unlock
	this->m_bIsCooling = false;
}
