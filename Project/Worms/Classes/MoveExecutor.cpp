#pragma once
#include "MoveExecutor.h"
#include "JYObject.h"
#include "ExecutorList.h"
#include "Inputhandler.h"
#include "MyScene.h"
#include "CoordinateConverter.h"
#include "InGamePacket.pb.h"
#include "Packet.h"
#include "ConnectionManager.h"

void MoveExecutor::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){

}

void MoveExecutor::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
}

void MoveExecutor::tick(float fDeltaTime){
	cocos2d::EventKeyboard::KeyCode keyCode;
	static float fPassedTime = 0;
	fPassedTime += fDeltaTime;
	if (fPassedTime >= 0.1f){
		float fDirection = this->getOwner()->getDirection();
		float fVelolcity = this->getOwner()->getVelocity();
		float fDistance = fVelolcity * fPassedTime;
		cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
		cocos2d::CCPoint currPos = pCCOwner->getPosition();
		cocos2d::CCPoint nextPos = cocos2d::ccp(currPos.x + fDistance *cos(fDirection), currPos.y + fDistance * tan(fDirection));
		this->setPlayerPosition(currPos, nextPos);
		fPassedTime = 0.0f;
	}
}

void MoveExecutor::setPlayerPosition(cocos2d::CCPoint src, cocos2d::CCPoint dst){
	cocos2d::CCActionInterval* moveAct = cocos2d::CCMoveTo::create(0.1f, dst);
	this->getOwner()->getCCObject()->runAction(moveAct);
}


void MoveExecutor::onMouseMove(cocos2d::Event* pEvent){

}

void MoveExecutor::onMouseDown(cocos2d::Event* pEvent){

}
