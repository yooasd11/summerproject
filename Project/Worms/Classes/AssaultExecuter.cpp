#pragma once
#include "AssaultExecuter.h"
#include "MyScene.h"
#include "JYArm.h"
#include "JYPlayer.h"
#include "CoordinateConverter.h"
#include "InGamePacket.pb.h"
#include "Packet.h"
#include "ConnectionManager.h"

void AssaultExecuter::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	if (cocos2d::EventKeyboard::KeyCode::KEY_1 <= keyCode && keyCode <= cocos2d::EventKeyboard::KeyCode::KEY_1){
	}
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
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;
	JYArm* pJYBullet = (JYArm*)pOwner->getChildByName("JYBullet");
	if (pJYBullet == nullptr) return;

	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCSprite* pCCBullet = (cocos2d::CCSprite*)pJYBullet->getCCObject();
	cocos2d::CCSprite* pAim = (cocos2d::CCSprite*)pCCOwner->getChildByName("Aim");
	cocos2d::CCPoint bulletPos = pAim->convertToWorldSpace(pAim->getPosition());

	float fDirection = CoordinateConverter::getInstance()->getDegreeBetweenCCNodeAndMouse(pCCBullet, pEvent);

	pCCBullet->setPosition(bulletPos);
	pCCBullet->setRotation(fDirection);
	pCCBullet->setVisible(true);

	char sendBuf[PKTLENGTH];
	InGamePacket::C_Shoot c_shoot;
	c_shoot.set_uid(this->getOwner()->getUID());
	c_shoot.set_x(pCCOwner->getPosition().x);
	c_shoot.set_y(pCCOwner->getPosition().y);
	c_shoot.set_damage(10.0f);
	c_shoot.set_direction(fDirection);
	c_shoot.set_velocity(50.0f);

	c_shoot.SerializeToArray(sendBuf, c_shoot.ByteSize());
	
	ConnectionManager::getInstance()->transmit(c_shoot.ByteSize(), PACKET_TYPE::PKT_C_SHOOT, sendBuf);
}