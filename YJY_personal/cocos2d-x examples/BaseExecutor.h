#pragma once

#include "cocos2d.h"
#include "ExecutorList.h"

class JYObject;
enum __Executor;

class BaseExecutor{
public:
	virtual char* getName() = 0;
	virtual __Executor getEnum() = 0;
private:
	JYObject* m_pOwner;
public:
	BaseExecutor(JYObject* pOwner);
	virtual ~BaseExecutor();

	void regist();

	JYObject* const getOwner() const;
	void setOwner(JYObject* pOwner);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime);
};

#define GENERATE_FUNC(PARAM) \
	public: \
	virtual char* getName() override { return #PARAM; } \
	virtual __Executor getEnum() override { return __##PARAM; } \
	private: 