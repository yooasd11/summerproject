#pragma once
#include "JYObject.h"

class JYPlayer : public JYObject{
private:
public:
	JYPlayer(cocos2d::CCNode* pCCObject);
	virtual ~JYPlayer() {}

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual	void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
};