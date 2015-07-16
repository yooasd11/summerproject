#include "CollisionExecutor.h"
#include "JYObject.h"

CollisionExecutor::CollisionExecutor(JYObject* pJYObject) : BaseExecutor(pJYObject){
	regist();
}

CollisionExecutor::~CollisionExecutor(){
}

cocos2d::CCPoint CollisionExecutor::tileCoordForPostion(cocos2d::CCPoint pos){
	cocos2d::CCLayer* nowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)nowScene->getChildByName("Background")->getChildByName("Tmap");
	int x = pos.x / pTmap->getTileSize().width;
	int y = (pTmap->getMapSize().height * pTmap->getTileSize().height - pos.y) / pTmap->getTileSize().height;
	return cocos2d::ccp(x, y);
}

bool CollisionExecutor::boundaryCollisionChecker(cocos2d::CCPoint& pos){
	cocos2d::CCLayer* nowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)nowScene->getChildByName("Background")->getChildByName("Tmap");
	float tmapWidth = pTmap->getMapSize().width * pTmap->getTileSize().width;
	float tmapHeight = pTmap->getMapSize().height * pTmap->getTileSize().height;
	//정상적인 좌표
	if (pos.x > 0 && pos.x < tmapWidth &&
		pos.y > 0 && pos.y < tmapHeight)
		return false;
	return true;
}

bool CollisionExecutor::objectCollisionChecker(cocos2d::CCPoint& pos){
	cocos2d::CCLayer* nowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)nowScene->getChildByName("Background")->getChildByName("Tmap");
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

void CollisionExecutor::tick(float fDeltaTime){
	cocos2d::CCLayer* nowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCTMXTiledMap* pTmap = (cocos2d::CCTMXTiledMap*)nowScene->getChildByName("Background")->getChildByName("Tmap");
	cocos2d::CCNode* pCCOwner = this->getOwner()->getCCObject();
	cocos2d::CCPoint currentPos = pTmap->convertToNodeSpace(pCCOwner->getPosition());
	if (boundaryCollisionChecker(currentPos) == true || objectCollisionChecker(currentPos) == true){
		pCCOwner->setPosition(cocos2d::ccp(5.0f, 5.0f));
		pCCOwner->setVisible(false);
		currentPos = pTmap->convertToNodeSpace(pCCOwner->getPosition());
	}
}