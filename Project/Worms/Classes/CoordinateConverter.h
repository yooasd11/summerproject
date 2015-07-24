#pragma once
#include "cocos2d.h";
#include "MyScene.h"

class CoordinateConverter{
private:
	CoordinateConverter() {}
	~CoordinateConverter() {}
public:
	static CoordinateConverter* getInstance(){
		static CoordinateConverter instance;
		return &instance;
	}

	cocos2d::CCPoint convertToTmapPos(cocos2d::CCNode* pCCNode){
		cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
		cocos2d::CCPoint nodeWorldPos = pCCNode->getParent()->convertToWorldSpace(pCCNode->getPosition());
		return pTmap->convertToNodeSpace(nodeWorldPos);
	}

	cocos2d::CCPoint getMouseGlobalPos(cocos2d::Event* pEvent){
		cocos2d::EventMouse* e = (cocos2d::EventMouse*)pEvent;
		cocos2d::CCTMXTiledMap* pTmap = GET_TMAP;
		cocos2d::CCPoint mouseGLPos = e->getLocationInView();
		mouseGLPos.y += 640.0f;

		return mouseGLPos;
	}

	//returns degree between VIEW CCPoint and mouse cursur location
	float getDegreeBetweenCCPosAndMouse(cocos2d::CCPoint pos, cocos2d::Event* pEvent){
		cocos2d::CCPoint mouseWinPos = getMouseGlobalPos(pEvent);
		float ret = MATH_RAD_TO_DEG(atan2(mouseWinPos.x - pos.x, mouseWinPos.y - pos.y)) - 90.0f;
		ret *= -1.0f;
		CCLOG("Node : (%.2f, %.2f) / Mouse : (%.2f, %.2f) / degree : %.2f", pos.x, pos.y, mouseWinPos.x, mouseWinPos.y, ret);
		return ret;
	}
};