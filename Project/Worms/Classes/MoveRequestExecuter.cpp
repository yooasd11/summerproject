#pragma once
#include "MoveRequestExecuter.h"
#include "JYObject.h"
#include "Packet.h"
#include "InGamePacket.pb.h"
#include "ConnectionManager.h"

void MoveRequestExecuter::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
	float fVelocity = 20.0f;
	float fDirection = 0.0f;
	float fX = pDragon->getPosition().x;
	float fY = pDragon->getPosition().y;
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A){
		fDirection = 180.0f;
	}
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D){
		fDirection = 0.0f;
	}

	char sendBuf[PKTBODYSIZE];
	InGamePacket::C_Move c_move;
	c_move.set_uid(this->getOwner()->getUID());
	c_move.set_velocity(fVelocity);
	c_move.set_x(fX);
	c_move.set_y(fY);
	c_move.set_direction(fDirection);
	c_move.SerializeToArray(sendBuf, c_move.ByteSize());

	ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
}

void MoveRequestExecuter::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
	float fX = pDragon->getPosition().x;
	float fY = pDragon->getPosition().y;

	char sendBuf[PKTBODYSIZE];
	InGamePacket::C_Stop c_stop;
	c_stop.set_uid(this->getOwner()->getUID());
	c_stop.set_x(fX);
	c_stop.set_y(fY);

	c_stop.SerializeToArray(sendBuf, c_stop.ByteSize());

	ConnectionManager::getInstance()->transmit(c_stop.ByteSize(), PACKET_TYPE::PKT_C_STOP, sendBuf);
}