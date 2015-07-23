#pragma once
#include "BaseExecutor.h"

class MoveRequestExecutor : public BaseExecutor{
	GENERATE_FUNC(MoveRequestExecutor);
private:
public:
	MoveRequestExecutor(JYObject* pJYObject) : BaseExecutor(pJYObject){
		regist();
	};
	virtual ~MoveRequestExecutor() {};

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
};