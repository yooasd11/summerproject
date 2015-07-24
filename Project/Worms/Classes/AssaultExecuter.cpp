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
	cocos2d::CCPoint aimPos = pFireAim->getParent()->convertToWorldSpace(pFireAim->getPosition());
	float degree = CoordinateConverter::getInstance()->getDegreeBetweenCCPosAndMouse(aimPos, pEvent);

	pFireAim->setRotation(degree);
}

void AssaultExecuter::onMouseDown(cocos2d::Event* pEvent){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	cocos2d::CCPoint ownerWorldPos = pTmap->convertToWorldSpace(pCCOwner->getPosition());
	cocos2d::CCPoint ownerTmapPos = pTmap->convertToNodeSpace(ownerWorldPos);
	float fDirection = CoordinateConverter::getInstance()->getDegreeBetweenCCPosAndMouse(ownerWorldPos, pEvent);

	char sendBuf[PKTLENGTH];
	InGamePacket::C_Shoot c_shoot;
	c_shoot.set_uid(this->getOwner()->getUID());
	c_shoot.set_th(0);
	c_shoot.set_x(ownerTmapPos.x);
	c_shoot.set_y(ownerTmapPos.y);
	c_shoot.set_damage(10.0f);
	c_shoot.set_direction(fDirection);
	c_shoot.set_velocity(30.0f);

	c_shoot.SerializeToArray(sendBuf, c_shoot.ByteSize());
	
	ConnectionManager::getInstance()->transmit(c_shoot.ByteSize(), PACKET_TYPE::PKT_C_SHOOT, sendBuf);
}
