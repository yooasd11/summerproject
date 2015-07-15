#include "MoveExecutor.h"
#include "JYObject.h"
#include "ExecutorList.h"
#include "Inputhandler.h"
#include "MyScene.h"

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
	if (InputHandler::getInstance()->isKeyPressed(keyCode = cocos2d::EventKeyboard::KeyCode::KEY_A) == true)
		handleKeycodeMovement(keyCode);
	if (InputHandler::getInstance()->isKeyPressed(keyCode = cocos2d::EventKeyboard::KeyCode::KEY_D) == true)
		handleKeycodeMovement(keyCode);
}

void MoveExecutor::handleKeycodeMovement(cocos2d::EventKeyboard::KeyCode keyCode){
	int nKeyCode = (int)keyCode;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)(getOwner()->getCCObject());
	cocos2d::CCPoint currPos = pCCOwner->getPosition();
	//key에 액션에 아직 이루어 지고 있는가 check
	if (pCCOwner->getActionByTag(nActionTagForKeycode[nKeyCode]) == nullptr){
		nActionTagForKeycode[nKeyCode] = 0;
		cocos2d::CCPoint nextPos = getNextPos(currPos, keyCode);
		setPlayerPosition(currPos, nextPos, keyCode);
	}
}

cocos2d::CCPoint MoveExecutor::getNextPos(cocos2d::CCPoint pos, cocos2d::EventKeyboard::KeyCode keyCode){
	int nKeyCode = (int)keyCode;
	if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_W) return pos + cocos2d::ccp(0.0f, 10.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_S) return pos + cocos2d::ccp(0.0f, -10.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_A) return pos + cocos2d::ccp(-10.0f, 0.0f);
	else if (nKeyCode == (int)cocos2d::EventKeyboard::KeyCode::KEY_D) return pos + cocos2d::ccp(10.0f, 0.0f);
	return pos;
}

void MoveExecutor::setPlayerPosition(cocos2d::CCPoint src, cocos2d::CCPoint dst, cocos2d::EventKeyboard::KeyCode keyCode){
	//충돌 체크
	boundaryCollisionChecker(src, dst);
	objectCollisionChecker(src, dst);

	//action tag를 key에 부여한 후 수행한다.
	static int nGenerateActionTag = 1000;
	cocos2d::CCActionInterval* moveAct = cocos2d::CCMoveTo::create(0.05f, dst);
	moveAct->setTag(++nGenerateActionTag);
	this->getOwner()->getCCObject()->runAction(moveAct);
	nActionTagForKeycode[(int)keyCode] = nGenerateActionTag;
}

bool MoveExecutor::boundaryCollisionChecker(cocos2d::CCPoint& src, cocos2d::CCPoint& dst){
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)this->getOwner()->getCCObject()->getParent();
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
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)this->getOwner()->getCCObject()->getParent();
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
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)this->getOwner()->getCCObject()->getParent();
	int x = pos.x / pTmap->getTileSize().width;
	int y = (pTmap->getMapSize().height * pTmap->getTileSize().height - pos.y) / pTmap->getTileSize().height;
	return cocos2d::ccp(x, y);
}

void MoveExecutor::onMouseMove(cocos2d::Event* pEvent){
	cocos2d::EventMouse* e = (cocos2d::EventMouse*)pEvent;
	cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getOwner()->getCCObject();
	cocos2d::CCSprite* fireAim = (cocos2d::CCSprite*)pDragon->getChildByName("Aim");

	cocos2d::CCScene* currentScene = cocos2d::CCDirector::getInstance()->getRunningScene();

	cocos2d::CCPoint aimWinPos = pDragon->convertToWorldSpace(fireAim->getPosition() - fireAim->getAnchorPoint());
	cocos2d::CCPoint mouseWinPos = currentScene->convertToWorldSpace(e->getLocation());

	mouseWinPos.y -= currentScene->getContentSize().height * 2;
	mouseWinPos.y = abs(mouseWinPos.y);

	float degree = MATH_RAD_TO_DEG(atan2(mouseWinPos.x - aimWinPos.x, mouseWinPos.y - aimWinPos.y)) - 90.0f;
	fireAim->setRotation(degree);

	CCLOG("Mouse : (%.2f, %.2f), Aim : (%.2f, %.2f), Degree : %.2f", mouseWinPos.x, mouseWinPos.y, aimWinPos.x, aimWinPos.y, degree);
}