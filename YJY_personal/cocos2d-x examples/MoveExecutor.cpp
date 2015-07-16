#pragma once
#include "MoveExecutor.h"
#include "JYObject.h"
#include "ExecutorList.h"
#include "Inputhandler.h"
#include "MyScene.h"
#include "CoordinateConverter.h"

void MoveExecutor::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
}

void MoveExecutor::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
}

void MoveExecutor::tick(float fDeltaTime){
	cocos2d::EventKeyboard::KeyCode keyCode;
	if (InputHandler::getInstance()->isKeyPressed(keyCode = cocos2d::EventKeyboard::KeyCode::KEY_W) == true)  
		handleKeycodeMovement(keyCode);
	if (InputHandler::getInstance()->isKeyPressed(keyCode = cocos2d::EventKeyboard::KeyCode::KEY_S) == true)
		handleKeycodeMovement(keyCode);
	if (InputHandler::getInstance()->isKeyPressed(keyCode = cocos2d::EventKeyboard::KeyCode::KEY_A) == true){
		cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
		pDragon->setFlippedX(false);
		handleKeycodeMovement(keyCode);
	}
	if (InputHandler::getInstance()->isKeyPressed(keyCode = cocos2d::EventKeyboard::KeyCode::KEY_D) == true){
		cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
		pDragon->setFlippedX(true);
		handleKeycodeMovement(keyCode);
	}
}

void MoveExecutor::handleKeycodeMovement(cocos2d::EventKeyboard::KeyCode keyCode){
	int nKeyCode = (int)keyCode;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)(getOwner()->getCCObject());
	cocos2d::CCPoint currPos = pCCOwner->getPosition();
	//key�� �׼ǿ� ���� �̷�� ���� �ִ°� check
	if (pCCOwner->getActionByTag(nActionTagForKeycode[nKeyCode]) == nullptr){
		nActionTagForKeycode[nKeyCode] = 0;
		cocos2d::CCPoint nextPos = getNextPos(currPos, keyCode);
		setPlayerPosition(currPos, nextPos, keyCode);
	}
}

cocos2d::CCPoint MoveExecutor::getNextPos(cocos2d::CCPoint pos, cocos2d::EventKeyboard::KeyCode keyCode){
	int nKeyCode = (int)keyCode;
	cocos2d::CCPoint nextRelPos = ccp(0.0f, 0.0f);
	if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_W) 
		nextRelPos = cocos2d::ccp(0.0f, 10.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_S)
		nextRelPos = cocos2d::ccp(0.0f, -10.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_A) 
		nextRelPos = cocos2d::ccp(-10.0f, 0.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_D) 
		nextRelPos = cocos2d::ccp(10.0f, 0.0f);
	return pos + nextRelPos;
}

void MoveExecutor::setPlayerPosition(cocos2d::CCPoint src, cocos2d::CCPoint dst, cocos2d::EventKeyboard::KeyCode keyCode){
	boundaryCollisionChecker(src, dst);
	objectCollisionChecker(src, dst);
	//CollisionExecutor* pCollisionExecutor = getOwner()->getExecutor(__Executor::__CollisionExecutor);

	//action tag�� key�� �ο��� �� �����Ѵ�.
	static int nGenerateActionTag = 1000;
	cocos2d::CCActionInterval* moveAct = cocos2d::CCMoveTo::create(0.05f, dst);
	moveAct->setTag(++nGenerateActionTag);
	this->getOwner()->getCCObject()->runAction(moveAct);
	nActionTagForKeycode[(int)keyCode] = nGenerateActionTag;
}

bool MoveExecutor::boundaryCollisionChecker(cocos2d::CCPoint& src, cocos2d::CCPoint& dst){
	cocos2d::CCLayer* nowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)nowScene->getChildByName("Background")->getChildByName("Tmap");
	float tmapWidth = pTmap->getMapSize().width * pTmap->getTileSize().width;
	float tmapHeight = pTmap->getMapSize().height * pTmap->getTileSize().height;
	//�������� ��ǥ
	if (dst.x >= 0 && dst.x <= tmapWidth &&
		dst.y >= 0 && dst.y <= tmapHeight)
		return false;
	//x�� ������ ��
	if (dst.x < 0 || dst.x > tmapWidth)
		dst.x = src.x;
	//y�� ������ ��
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

	int tileGid = metaInfo->getTileGIDAt(tileCoord);		//�ش� tile�� gid�� �޾ƿ�

	if (tileGid){
		cocos2d::Value& properties = pTmap->getPropertiesForGID(tileGid);		//�ش� gid�� �Ӽ��� �޾ƿ�
		//�Ӽ��� ���� ó��
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