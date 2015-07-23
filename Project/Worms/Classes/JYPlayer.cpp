#pragma once
#include "JYPlayer.h"
#include "MoveRequestExecutor.h"
#include "MoveExecutor.h"
#include "AssaultExecutor.h"
#include "CollisionExecutor.h"
#include "CoordinateConverter.h"
#include "Packet.h"
#include "Inputhandler.h"

JYPlayer::JYPlayer(cocos2d::CCNode* pCCObject) : JYObject(pCCObject){
	new MoveRequestExecutor(this);
	new MoveExecutor(this);
	new CollisionExecutor(this);
	new AssaultExecutor(this);
	this->setObjectType(JYOBJECT_TYPE::JY_PLAYER);
}
void JYPlayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	MoveRequestExecutor* pMoveRequestExecutor = (MoveRequestExecutor*)this->getExecutor(__Executor::__MoveRequestExecutor);
	if (pMoveRequestExecutor != nullptr)
	{
		pMoveRequestExecutor->onKeyPressed(keyCode, pEvent);
	}
}

void JYPlayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	MoveRequestExecutor* pMoveRequestExecutor = (MoveRequestExecutor*)this->getExecutor(__Executor::__MoveRequestExecutor);
	if (pMoveRequestExecutor != nullptr)
	{
		pMoveRequestExecutor->onKeyReleased(keyCode, pEvent);
	}
}

void JYPlayer::onMouseMove(cocos2d::Event* pEvent){
	AssaultExecutor* pAssaultExecutor = (AssaultExecutor*)this->getExecutor(__Executor::__AssaultExecutor);
	if (pAssaultExecutor != nullptr)
	{
		pAssaultExecutor->onMouseMove(pEvent);
	}
}

void JYPlayer::onMouseDown(cocos2d::Event* pEvent){
	AssaultExecutor* pAssaultExecutor = (AssaultExecutor*)this->getExecutor(__Executor::__AssaultExecutor);
	if (pAssaultExecutor != nullptr){
		pAssaultExecutor->onMouseDown(pEvent);
	}
}
