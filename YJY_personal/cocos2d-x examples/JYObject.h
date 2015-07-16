#pragma once

#include "cocos2d.h"
#include "ExecutorList.h"
#include "BaseExecutor.h"

class JYObject{
private:
	cocos2d::CCNode* m_pCCObject;
	JYObject* m_pJYObjectParent;
	BaseExecutor* m_Executors[__MaxExecutor];
	std::vector<JYObject*> m_pJYChildrenVector;
	std::string m_sName;
	std::vector<JYObject*>::iterator getChildIteratorByName(const std::string& sName);
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

	void setName(const std::string& sName) { m_sName = sName; }
	std::string getName() { return m_sName; }
	JYObject* getParent() { return m_pJYObjectParent; }
	void addChild(JYObject* const pJYObject);
	JYObject* getChildByName(const std::string& sName);
	void removeChildByName(const std::string& sName);
	
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual	void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime);
};