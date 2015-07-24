#include "CollisionExecuter.h"
#include "JYObject.h"
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
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)pMyScene->getChildByName("Background")->getChildByName("Tmap");
	int x = pos.x / pTmap->getTileSize().width;
	int y = (pTmap->getMapSize().height * pTmap->getTileSize().height - pos.y) / pTmap->getTileSize().height;
	return cocos2d::ccp(x, y);
}

bool CollisionExecuter::boundaryCollisionChecker(cocos2d::CCPoint& pos){
	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)pMyScene->getChildByName("Background")->getChildByName("Tmap");
	float tmapWidth = pTmap->getMapSize().width * pTmap->getTileSize().width - 1;
	float tmapHeight = pTmap->getMapSize().height * pTmap->getTileSize().height - 1;
	//�������� ��ǥ
	if (pos.x > 1 && pos.x < tmapWidth &&
		pos.y > 1 && pos.y < tmapHeight )
		return false;
	return true;
}

bool CollisionExecuter::objectCollisionChecker(cocos2d::CCPoint& pos){
	MyScene* pMyScene = GET_MYSCENE;
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)pMyScene->getChildByName("Background")->getChildByName("Tmap");
	cocos2d::CCPoint tileCoord = this->tileCoordForPostion(pos);
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
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)pMyScene->getChildByName("Background")->getChildByName("Tmap");
	cocos2d::CCPoint currentPos = pTmap->convertToNodeSpace(pCCOwner->getPosition());
	cocos2d::CCPoint nextPos = this->getNextPos(currentPos, fDeltaTime);
	if (boundaryCollisionChecker(nextPos) == true || objectCollisionChecker(nextPos) == true){
		char sendBuf[PKTBODYSIZE];
		InGamePacket::C_Stop c_stop;

		if (nJYObjectType == JYOBJECT_TYPE::JY_ARM){
			pCCOwner->setVisible(false);
			c_stop.set_type(JYOBJECT_TYPE::JY_ARM);
			c_stop.set_th(this->getOwner()->getTag());
		}
		else if (nJYObjectType == JYOBJECT_TYPE::JY_PLAYER){
			this->getOwner()->setVelocity(0.0f);
			pCCOwner->setPosition(currentPos);
			c_stop.set_type(JYOBJECT_TYPE::JY_ARM);
		}

		c_stop.set_uid(this->getOwner()->getUID());
		c_stop.set_x(currentPos.x);
		c_stop.set_y(currentPos.y);
		c_stop.SerializeToArray(sendBuf, c_stop.ByteSize());

		ConnectionManager::getInstance()->transmit(c_stop.ByteSize(), PACKET_TYPE::PKT_C_STOP, sendBuf);
	}
}