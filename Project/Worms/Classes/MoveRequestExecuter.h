#pragma once
#include "BaseExecuter.h"

class MoveRequestExecuter : public BaseExecuter{
	GENERATE_FUNC(MoveRequestExecuter);
private:
public:
	MoveRequestExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject){
		regist();
	};
	virtual ~MoveRequestExecuter() {};

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
};