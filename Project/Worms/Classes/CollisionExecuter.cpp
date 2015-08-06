#include "CollisionExecuter.h"
#include "AccelerationExecuter.h"
#include "JYObject.h"
#include "JYPlayer.h"
#include "JYArm.h"
#include "MyScene.h"
#include "ConnectionManager.h"
#include "InGamePacket.pb.h"
#include "Packet.h"

cocos2d::CCPoint CollisionExecuter::getNextPos(cocos2d::CCPoint currentPos, float fDeltaTime){
	float fVx = this->getOwner()->getVelocityX();
	float fVy = this->getOwner()->getVelocityY();
	float x = currentPos.x + fVx * fDeltaTime;
	float y = currentPos.y + fVy * fDeltaTime;
	return cocos2d::ccp(x, y);
}

cocos2d::CCPoint CollisionExecuter::tileCoordForPostion(cocos2d::CCPoint pos){
	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	int x = pos.x / pTmap->getTileSize().width;
	int y = (pTmap->getMapSize().height * pTmap->getTileSize().height - pos.y) / pTmap->getTileSize().height;
	return cocos2d::ccp(x, y);
}

bool CollisionExecuter::boundaryCollisionChecker(cocos2d::CCPoint& pos){
	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	float tmapWidth = pTmap->getMapSize().width * pTmap->getTileSize().width - 1;
	float tmapHeight = pTmap->getMapSize().height * pTmap->getTileSize().height - 1;

	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return false;
	cocos2d::CCNode* pCCOwner = pOwner->getCCObject();
	if (pCCOwner == nullptr) return false;
	JYOBJECT_TYPE eType = pOwner->getObjectType();
	//non-valid point
	if (pos.x < 1.0f)
		pCCOwner->setPosition(cocos2d::ccp(1.0f, pCCOwner->getPosition().y));
	if (pos.x >= tmapWidth)
		pCCOwner->setPosition(cocos2d::ccp(tmapWidth - 1, pCCOwner->getPosition().y));
	if (pos.y >= tmapHeight)
		pCCOwner->setPosition(cocos2d::ccp(pCCOwner->getPosition().x, tmapHeight - 1));
	if (eType == JYOBJECT_TYPE::JY_PLAYER && pos.y < 100.0f)
		pCCOwner->setPosition(pCCOwner->getPosition().x, 100.0f);
	if (eType == JYOBJECT_TYPE::JY_ARM && pos.y < 80.0f)
		pCCOwner->setPosition(pCCOwner->getPosition().x, 80.0f);

	return true;
}

bool CollisionExecuter::objectCollisionChecker(cocos2d::CCPoint& pos){
	//MyScene* pMyScene = GET_MYSCENE;
	//cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	//cocos2d::CCPoint tileCoord = this->tileCoordForPostion(pos);
	//cocos2d::CCTMXLayer* pLayerGround = pTmap->getLayer("Ground");

	//int groundTileGid = pLayerGround->getTileGIDAt(tileCoord);		//해당 tile의 gid를 받아옴

	//if (groundTileGid){
	//	cocos2d::Value& properties = pTmap->getPropertiesForGID(groundTileGid);		//해당 gid의 속성을 받아옴
	//	//속성에 따른 처리
	//	if (!properties.isNull()){
	//		std::string wall = properties.asValueMap()["Wall"].asString();
	//		if (wall == "YES"){
	//			CCLOG("Wall...");
	//			return true;
	//		}
	//	}
	//}
	return false;
}

void CollisionExecuter::tick(float fDeltaTime){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	cocos2d::CCPoint currentWorldPos = pCCOwner->getParent()->convertToWorldSpace(pCCOwner->getPosition());
	cocos2d::CCPoint currentPos = pTmap->convertToNodeSpace(currentWorldPos);
	cocos2d::CCPoint nextPos = this->getNextPos(currentPos, fDeltaTime);

	if (boundaryCollisionChecker(nextPos) == true || objectCollisionChecker(nextPos) == true){
	}
}