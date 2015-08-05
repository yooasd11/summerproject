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
		float fCurrentVx = pOwner->getVelocityX();
		c_move.set_unit_vx(0.0f);
		c_move.set_unit_vy(3.0f);
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
	InGamePacket::C_Move c_move;

	c_move.set_uid(this->getOwner()->getUID());

	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A){
		c_move.set_unit_vx(1.0f);
		c_move.set_unit_vy(0.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D){
		c_move.set_unit_vx(-1.0f);
		c_move.set_unit_vy(0.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}
}