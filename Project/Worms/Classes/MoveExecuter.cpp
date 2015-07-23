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
	if (fPassedTime >= 0.1f){
		float fDirection = this->getOwner()->getDirection();
		float fVelolcity = this->getOwner()->getVelocity();
		float fDistance = fVelolcity * fPassedTime;
		cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
		cocos2d::CCPoint currPos = pCCOwner->getPosition();
		cocos2d::CCPoint nextPos = cocos2d::ccp(currPos.x + fDistance *cos(fDirection), currPos.y + fDistance * sin(fDirection));
		this->setPlayerPosition(currPos, nextPos);
		fPassedTime = 0.0f;
	}
}

void MoveExecuter::setPlayerPosition(cocos2d::CCPoint src, cocos2d::CCPoint dst){
	cocos2d::CCActionInterval* moveAct = cocos2d::CCMoveTo::create(0.1f, dst);
	this->getOwner()->getCCObject()->runAction(moveAct);
}


void MoveExecuter::onMouseMove(cocos2d::Event* pEvent){

}

void MoveExecuter::onMouseDown(cocos2d::Event* pEvent){

}