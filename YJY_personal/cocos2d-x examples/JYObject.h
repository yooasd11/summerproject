#pragma once

#include "cocos2d.h"
#include "ExecutorList.h"
#include "BaseExecutor.h"

class JYObject{
private:
	cocos2d::CCNode* m_pCCObject;
	BaseExecutor* m_Executors[__MaxExecutor];
public:
	JYObject(cocos2d::CCNode* pCCObject);
	virtual ~JYObject();

	BaseExecutor* getExecutor(int type) { 
		return m_Executors[type]; 
	}
	void setExecutor(BaseExecutor* param);
	void ReleaseExecutor(BaseExecutor* param);

	cocos2d::CCNode* getCCObject() { return m_pCCObject; }
	void setCCObjet(cocos2d::CCNode* ccOb) { m_pCCObject = ccOb; }
	
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
		m_Executors[__MoveExecutor]->onKeyPressed(keyCode, pEvent);
	}
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
		m_Executors[__MoveExecutor]->onKeyReleased(keyCode, pEvent);
	}

	virtual void onMouseMove(cocos2d::Event* pEvent){
		m_Executors[__MoveExecutor]->onMouseMove(pEvent);
	}

	virtual void tick(float fDeltaTime);
};