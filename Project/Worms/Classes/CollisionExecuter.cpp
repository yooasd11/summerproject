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

	//non-valid point
	if( pos.x < 1.0f ||
		pos.x >= tmapWidth ||
		pos.y >= tmapHeight)
		return true;
	JYOBJECT_TYPE eType = pOwner->getObjectType();
	if ((eType == JYOBJECT_TYPE::JY_PLAYER && pos.y < 95.0f) ||
		(eType == JYOBJECT_TYPE::JY_ARM && pos.y < 80.0f))
		return true;
	return false;
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
		JYOBJECT_TYPE eType = pOwner->getObjectType();
		if (eType == JYOBJECT_TYPE::JY_PLAYER)
			pCCOwner->setPosition(pCCOwner->getPosition().x, 100.0f);
		else if (eType == JYOBJECT_TYPE::JY_ARM)
			pCCOwner->setPosition(pCCOwner->getPosition().x, 80.0f);
 		/*AccelerationExecuter* pAE = (AccelerationExecuter*)pOwner->getExecuter(__Executer::__AccelerationExecuter);
		pAE->clearAcceleration();
		pOwner->setVelocity(0.0f, 0.0f);
		char sendBuf[PKTBODYSIZE];
		InGamePacket::C_Collision cCollisionPacket;

		cCollisionPacket.set_uid1(pOwner->getUID());
		
		cCollisionPacket.SerializeToArray(sendBuf, cCollisionPacket.ByteSize());

		ConnectionManager::getInstance()->transmit(cCollisionPacket.ByteSize(), PACKET_TYPE::PKT_C_COLLISION, sendBuf);*/
	}
}