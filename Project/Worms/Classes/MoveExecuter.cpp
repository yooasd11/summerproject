#pragma once
#include "MoveExecuter.h"
#include "JYObject.h"
#include "ExecuterList.h"
#include "Inputhandler.h"
#include "MyScene.h"
#include "CoordinateConverter.h"
#include "InGamePacket.pb.h"
#include "Packet.h"
#include "ConnectionManager.h"

void MoveExecuter::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){

}

void MoveExecuter::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
}

void MoveExecuter::tick(float fDeltaTime){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	cocos2d::CCPoint currPos = pCCOwner->getPosition();

	float fDX = pOwner->getVelocityX() * fDeltaTime;
	float fDY = pOwner->getVelocityY() * fDeltaTime;

	cocos2d::CCPoint nextPos = pCCOwner->getPosition() + cocos2d::ccp(fDX, fDY);
	pCCOwner->setPosition(nextPos);
}

void MoveExecuter::onMouseMove(cocos2d::Event* pEvent){

}

void MoveExecuter::onMouseDown(cocos2d::Event* pEvent){

}
