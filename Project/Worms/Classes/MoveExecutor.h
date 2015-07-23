#pragma once
#include "cocos2d.h"
#include "BaseExecutor.h"

class MoveExecutor : public BaseExecutor{
	GENERATE_FUNC(MoveExecutor)
private:
	void setPlayerPosition(cocos2d::CCPoint src, cocos2d::CCPoint dst);
public:
	MoveExecutor(JYObject* pOwner) : BaseExecutor(pOwner) {
		regist();
	}
	virtual ~MoveExecutor() {}
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime);
};

