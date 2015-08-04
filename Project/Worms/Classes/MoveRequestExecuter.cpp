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
	c_move.set_unit_vy(0.0f);


	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A){
		c_move.set_unit_vx(-1.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D){
		c_move.set_unit_vx(1.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}

	//JUMP!
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE){
		//acceleration setting
		//AccelerationExecuter* pAccelerationExecuter = (AccelerationExecuter*)pOwner->getExecuter(__Executer::__AccelerationExecuter);
		//pAccelerationExecuter->addAcceleration(0.0f, 50.0f);
	}

}

void MoveRequestExecuter::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	/*if (InputHandler::getInstance()->isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_A) == true ||
		InputHandler::getInstance()->isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_D) == true)
		return;*/

	char sendBuf[PKTBODYSIZE];
	InGamePacket::C_Move c_move;

	c_move.set_uid(this->getOwner()->getUID());
	c_move.set_unit_vy(0.0f);
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A){
		c_move.set_unit_vx(1.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D){
		c_move.set_unit_vx(-1.0f);
		c_move.SerializeToArray(sendBuf, c_move.ByteSize());
		ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
	}
}