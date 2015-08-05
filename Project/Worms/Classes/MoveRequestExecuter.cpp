#pragma once
#include "MoveRequestExecuter.h"
#include "JYObject.h"
#include "Packet.h"
#include "InGamePacket.pb.h"
#include "ConnectionManager.h"
#include "Inputhandler.h"

#include "AccelerationExecuter.h"

void MoveRequestExecuter::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	char sendBuf[PKTBODYSIZE];
	InGamePacket::C_Move c_move;
	c_move.set_uid(this->getOwner()->getUID());

	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A){
		c_move.set_unit_vx(-1.0f);
		c_move.set_unit_vy(0.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D){
		c_move.set_unit_vx(1.0f);
		c_move.set_unit_vy(0.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}

	//JUMP!
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE){
		//acceleration setting
		float fCurrentVx = pOwner->getVelocityX();
		c_move.set_unit_vx(0.0f);
		if (fCurrentVx > 0.0f)
			c_move.set_unit_vx(1.0f);
		else if (fCurrentVx < 0.0f)
			c_move.set_unit_vx(-1.0f);
		c_move.set_unit_vy(5.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}

}

void MoveRequestExecuter::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	float fX = pCCOwner->getPosition().x;
	float fY = pCCOwner->getPosition().y;

	char sendBuf[PKTBODYSIZE];
	InGamePacket::C_Stop c_stop;

	c_stop.set_uid(this->getOwner()->getUID());
	c_stop.set_x(fX);
	c_stop.set_y(fY);

	c_stop.SerializeToArray(sendBuf, c_stop.ByteSize());

	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D)
		ConnectionManager::getInstance()->transmit(c_stop.ByteSize(), PACKET_TYPE::PKT_C_STOP, sendBuf);
}