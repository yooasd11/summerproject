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
	cocos2d::EventKeyboard::KeyCode keyCode;
	static float fPassedTime = 0;
	fPassedTime += fDeltaTime;
	if (fPassedTime >= 0.03f){
		JYObject* pOwner = this->getOwner();
		if (pOwner == nullptr) return;
		cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
		if (pCCOwner == nullptr) return;

		float fDirection = this->getOwner()->getDirection();
		float fVelolcity = this->getOwner()->getVelocity();
		float fDistance = fVelolcity * fPassedTime;

		cocos2d::CCPoint currPos = pCCOwner->getPosition();

		float fDX = fDistance *sin(MATH_DEG_TO_RAD(fDirection));
		float fDY = fDistance *cos(MATH_DEG_TO_RAD(fDirection));
		
		if (this->getOwner()->getObjectType() == JYOBJECT_TYPE::JY_ARM)
		CCLOG("dx : %.2f, dy : %.2f", fDX, fDY);	

		cocos2d::CCPoint nextPos = cocos2d::ccp(fDX, fDY);
		this->setPlayerPosition(nextPos);
		fPassedTime = 0.0f;
	}
}

void MoveExecuter::setPlayerPosition(cocos2d::CCPoint dst){
	cocos2d::CCActionInterval* moveAct = cocos2d::CCMoveBy::create(0.1f, dst);
	this->getOwner()->getCCObject()->runAction(moveAct);
}


void MoveExecuter::onMouseMove(cocos2d::Event* pEvent){

}

void MoveExecuter::onMouseDown(cocos2d::Event* pEvent){

}
