#pragma once
#include "cocos2d.h";

class CoordinateConverter{
private:
	CoordinateConverter() {}
	~CoordinateConverter() {}
public:
	static CoordinateConverter* getInstance(){
		static CoordinateConverter instance;
		return &instance;
	}

	cocos2d::CCPoint getMouseGlobalPos(cocos2d::Event* pEvent){
		cocos2d::EventMouse* e = (cocos2d::EventMouse*)pEvent;
		cocos2d::CCLayer* currentScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");

		cocos2d::CCPoint mouseWinPos = currentScene->convertToNodeSpace(e->getLocation());
		mouseWinPos.y -= currentScene->getContentSize().height * 2;
		mouseWinPos.y = abs(mouseWinPos.y);

		return mouseWinPos;
	}

	cocos2d::CCPoint getGlobalPos(cocos2d::CCNode* pCCNode){
		cocos2d::CCLayer* currentScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
		cocos2d::CCPoint ret = currentScene->convertToNodeSpace(pCCNode->getPosition());
		return ret;
	}

	float getDegreeBetweenCCNodeAndMouse(cocos2d::CCNode* pCCNode1, cocos2d::Event* pEvent){
		cocos2d::CCPoint mouseWinPos = getMouseGlobalPos(pEvent);
		cocos2d::CCPoint aimWinPos = pCCNode1->getParent()->convertToWorldSpace(pCCNode1->getPosition() - pCCNode1->getAnchorPoint());

		float ret = MATH_RAD_TO_DEG(atan2(mouseWinPos.x - aimWinPos.x, mouseWinPos.y - aimWinPos.y)) - 90.0f;
		return ret;
	}
};