#pragma once
#include "JYPlayer.h"
#include "MoveRequestExecuter.h"
#include "MoveExecuter.h"
#include "AssaultExecuter.h"
#include "CollisionExecuter.h"
#include "CoordinateConverter.h"
#include "Packet.h"
#include "Inputhandler.h"

JYPlayer::JYPlayer(cocos2d::CCNode* pCCObject) : JYObject(pCCObject){
	new MoveRequestExecuter(this);
	new MoveExecuter(this);
	new CollisionExecuter(this);
	new AssaultExecuter(this);
	this->setObjectType(JYOBJECT_TYPE::JY_PLAYER);
}

void JYPlayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	MoveRequestExecuter* pMoveRequestExecuter = (MoveRequestExecuter*)this->getExecuter(__Executer::__MoveRequestExecuter);
	if (pMoveRequestExecuter != nullptr){
		pMoveRequestExecuter->onKeyPressed(keyCode, pEvent);
	}

	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onKeyPressed(keyCode, pEvent);
	}
}

void JYPlayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	MoveRequestExecuter* pMoveRequestExecuter = (MoveRequestExecuter*)this->getExecuter(__Executer::__MoveRequestExecuter);
	if (pMoveRequestExecuter != nullptr){
		pMoveRequestExecuter->onKeyReleased(keyCode, pEvent);
	}

	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onKeyPressed(keyCode, pEvent);
	}
}

void JYPlayer::onMouseMove(cocos2d::Event* pEvent){
	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onMouseMove(pEvent);
	}
}

void JYPlayer::onMouseDown(cocos2d::Event* pEvent){
	AssaultExecuter* pAssaultExecuter = (AssaultExecuter*)this->getExecuter(__Executer::__AssaultExecuter);
	if (pAssaultExecuter != nullptr){
		pAssaultExecuter->onMouseDown(pEvent);
	}
}
