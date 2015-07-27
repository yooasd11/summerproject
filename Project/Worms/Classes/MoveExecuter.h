#pragma once
#include "BaseExecuter.h"

class MoveExecuter : public BaseExecuter{
	GENERATE_FUNC(MoveExecuter)
private:
	void setPlayerPosition(cocos2d::CCPoint dst);
public:
	MoveExecuter(JYObject* pOwner) : BaseExecuter(pOwner, __MoveExecuter) {
		regist();
	}
	virtual ~MoveExecuter() { BaseExecuter::~BaseExecuter(); }
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime);
};

