#include "CollisionExecuter.h"
#include "JYObject.h"
#include "JYPlayer.h"
#include "JYArm.h"
#include "MyScene.h"
#include "ConnectionManager.h"
#include "InGamePacket.pb.h"
#include "Packet.h"

cocos2d::CCPoint CollisionExecuter::getNextPos(cocos2d::CCPoint currentPos, float fDeltaTime){
	float fVelocity = this->getOwner()->getVelocity();
	float fDistance = fVelocity * fDeltaTime;
	float fDirection = this->getOwner()->getDirection();
	float x = currentPos.x + fDistance * cos(fDirection);
	float y = currentPos.y + fDistance * tan(fDirection);
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
	//정상적인 좌표
	if (pos.x > 1 && pos.x < tmapWidth &&
		pos.y > 1 && pos.y < tmapHeight )
		return false;
	return true;
}

bool CollisionExecuter::objectCollisionChecker(cocos2d::CCPoint& pos){
	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	cocos2d::CCPoint tileCoord = this->tileCoordForPostion(pos);
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
				return true;
			}
		}
	}
	return false;
}

void CollisionExecuter::tick(float fDeltaTime){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCSprite* pCCOwner = (cocos2d::CCSprite*)pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	UINT nJYObjectType = this->getOwner()->getObjectType();
	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
	cocos2d::CCPoint currentWorldPos = pCCOwner->getParent()->convertToWorldSpace(pCCOwner->getPosition());
	cocos2d::CCPoint currentPos = pTmap->convertToNodeSpace(currentWorldPos);
	cocos2d::CCPoint nextPos = this->getNextPos(currentPos, fDeltaTime);
	
	if (boundaryCollisionChecker(nextPos) == true || objectCollisionChecker(nextPos) == true){
		char sendBuf[PKTBODYSIZE];
		InGamePacket::C_Collision cCollisionPacket;

		cCollisionPacket.set_x(currentPos.x);
		cCollisionPacket.set_y(currentPos.y);

		if (nJYObjectType == JYOBJECT_TYPE::JY_PLAYER){
			JYPlayer* pJYPlayer = (JYPlayer*)pOwner;
			cCollisionPacket.set_uid1(pJYPlayer->getUID());
			cCollisionPacket.set_hp(pJYPlayer->getHP());
		}
		else if (nJYObjectType == JYOBJECT_TYPE::JY_ARM){
			if (pOwner->getParent() == nullptr) return;
			cCollisionPacket.set_uid1(pOwner->getParent()->getUID());
			cCollisionPacket.set_th(pOwner->getTag());
		}
		cCollisionPacket.SerializeToArray(sendBuf, cCollisionPacket.ByteSize());

		ConnectionManager::getInstance()->transmit(cCollisionPacket.ByteSize(), PACKET_TYPE::PKT_C_COLLISION, sendBuf);
	}
}