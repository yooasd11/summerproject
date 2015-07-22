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
	cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
	float fVelocity = 0.0f;
	float fX = pDragon->getPosition().x;
	float fY = pDragon->getPosition().y;
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A){
		fVelocity = -20.0f;
	}
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D){
		fVelocity = 20.0f;
	}

	char sendBuf[PKTBODYSIZE];
	InGamePacket::C_Move c_move;
	c_move.set_uid(this->getOwner()->getUID());
	c_move.set_velocity(fVelocity);
	c_move.set_x(fX);
	c_move.set_y(fY);
	c_move.SerializeToArray(sendBuf, c_move.ByteSize());

	ConnectionManager::getInstance()->transmit(c_move.ByteSize(), PACKET_TYPE::PKT_C_MOVE, sendBuf);
}

void MoveExecutor::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
	float fX = pDragon->getPosition().x;
	float fY = pDragon->getPosition().y;

	char sendBuf[PKTBODYSIZE];
	InGamePacket::C_Stop c_stop;
	c_stop.set_uid(this->getOwner()->getUID());
	c_stop.set_x(fX);
	c_stop.set_y(fY);

	c_stop.SerializeToArray(sendBuf, c_stop.ByteSize());


	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D){
		ConnectionManager::getInstance()->transmit(c_stop.ByteSize(), PACKET_TYPE::PKT_C_STOP, sendBuf);
	}
}

void MoveExecutor::tick(float fDeltaTime){
	cocos2d::EventKeyboard::KeyCode keyCode;
	static float fPassedTime = 0;
	fPassedTime += fDeltaTime;
	if (fPassedTime >= 0.1f){
		float fVelolcity = this->getOwner()->getVelocity();
		cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
		cocos2d::CCPoint currPos = pCCOwner->getPosition();
		cocos2d::CCPoint nextPos = cocos2d::ccp(currPos.x + fVelolcity * fPassedTime, currPos.y);
		this->setPlayerPosition(currPos, nextPos);
		fPassedTime = 0.0f;
	}
}

void MoveExecutor::handleKeycodeMovement(cocos2d::EventKeyboard::KeyCode keyCode){
	int nKeyCode = (int)keyCode;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)(getOwner()->getCCObject());
	cocos2d::CCPoint currPos = pCCOwner->getPosition();
	//key에 액션에 아직 이루어 지고 있는가 check
	if (pCCOwner->getActionByTag(nActionTagForKeycode[nKeyCode]) == nullptr){
		nActionTagForKeycode[nKeyCode] = 0;
		cocos2d::CCPoint nextPos = getNextPos(currPos, keyCode);
		setPlayerPosition(currPos, nextPos);
	}
}

cocos2d::CCPoint MoveExecutor::getNextPos(cocos2d::CCPoint pos, cocos2d::EventKeyboard::KeyCode keyCode){
	int nKeyCode = (int)keyCode;
	cocos2d::CCPoint nextRelPos = ccp(0.0f, 0.0f);
	if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_W) 
		nextRelPos = cocos2d::ccp(0.0f, 5.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_S)
		nextRelPos = cocos2d::ccp(0.0f, -5.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_A) 
		nextRelPos = cocos2d::ccp(-5.0f, 0.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_D) 
		nextRelPos = cocos2d::ccp(5.0f, 0.0f);
	return pos + nextRelPos;
}

void MoveExecutor::setPlayerPosition(cocos2d::CCPoint src, cocos2d::CCPoint dst){
	boundaryCollisionChecker(src, dst);
	objectCollisionChecker(src, dst);
	//CollisionExecutor* pCollisionExecutor = getOwner()->getExecutor(__Executor::__CollisionExecutor);

	//action tag를 key에 부여한 후 수행한다.
	static int nGenerateActionTag = 1000;
	cocos2d::CCActionInterval* moveAct = cocos2d::CCMoveTo::create(0.1f, dst);
	moveAct->setTag(++nGenerateActionTag);
	this->getOwner()->getCCObject()->runAction(moveAct);
}

bool MoveExecutor::boundaryCollisionChecker(cocos2d::CCPoint& src, cocos2d::CCPoint& dst){
	cocos2d::CCLayer* nowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)nowScene->getChildByName("Background")->getChildByName("Tmap");
	float tmapWidth = pTmap->getMapSize().width * pTmap->getTileSize().width;
	float tmapHeight = pTmap->getMapSize().height * pTmap->getTileSize().height;
	//정상적인 좌표
	if (dst.x >= 0 && dst.x <= tmapWidth &&
		dst.y >= 0 && dst.y <= tmapHeight)
		return false;
	//x가 나갔을 때
	if (dst.x < 0 || dst.x > tmapWidth)
		dst.x = src.x;
	//y가 나갔을 때
	if (dst.y < 0 || dst.y > tmapHeight)
		dst.y = src.y;
	return true;
}

bool MoveExecutor::objectCollisionChecker(cocos2d::CCPoint& src, cocos2d::CCPoint& dst){
	cocos2d::CCLayer* nowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)nowScene->getChildByName("Background")->getChildByName("Tmap");
	cocos2d::CCPoint tileCoord = this->tileCoordForPostion(dst);
	cocos2d::CCTMXLayer* metaInfo = pTmap->getLayer("MetaInfo");
	cocos2d::CCTMXLayer* items = pTmap->getLayer("Items");

	int tileGid = metaInfo->getTileGIDAt(tileCoord);		//해당 tile의 gid를 받아옴

	if (tileGid){
		cocos2d::Value& properties = pTmap->getPropertiesForGID(tileGid);		//해당 gid의 속성을 받아옴
		//속성에 따른 처리
		if (!properties.isNull()){
			std::string wall = properties.asValueMap()["Wall"].asString();
			if (wall == "YES"){
				CCLOG("Wall...");
				dst = src;
				return false;
			}
			std::string item = properties.asValueMap()["Item"].asString();
			if (item == "YES"){
				metaInfo->removeTileAt(tileCoord);
				items->removeTileAt(tileCoord);
				CCLOG("Watermelon!!");
			}
		}
	}
	return true;
}

cocos2d::CCPoint MoveExecutor::tileCoordForPostion(cocos2d::CCPoint pos){
	cocos2d::CCLayer* nowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)nowScene->getChildByName("Background")->getChildByName("Tmap");
	int x = pos.x / pTmap->getTileSize().width;
	int y = (pTmap->getMapSize().height * pTmap->getTileSize().height - pos.y) / pTmap->getTileSize().height;
	return cocos2d::ccp(x, y);
}

void MoveExecutor::onMouseMove(cocos2d::Event* pEvent){
	cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
	cocos2d::CCSprite* fireAim = (cocos2d::CCSprite*)pDragon->getChildByName("Aim");
	
	float degree = CoordinateConverter::getInstance()->getDegreeBetweenCCNodeAndMouse(fireAim, pEvent);

	fireAim->setRotation(degree);
}

void MoveExecutor::onMouseDown(cocos2d::Event* pEvent){

}
