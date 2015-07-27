#pragma once
#include "cocos2d.h"
#include "ExecuterList.h"
#include "BaseExecuter.h"
#include "JYObjectList.h"

class JYObject{
private:
	JYOBJECT_TYPE m_nObjectType;
	UINT m_nUID;
	UINT m_nTag;
	float m_fVelocity;
	float m_fDirection;
	cocos2d::CCNode* m_pCCObject;
	JYObject* m_pJYObjectParent;
	BaseExecuter* m_Executers[__Executer::__Executer_END];
	std::vector<JYObject*> m_pJYChildrenVector;
	std::string m_sName;
	std::vector<JYObject*>::iterator getChildIteratorByTag(const UINT& nTag);
	std::vector<JYObject*>::iterator getChildIteratorByName(const std::string& sName);
	std::vector<JYObject*>::iterator getChildIteratorByObject(JYObject* const pJYObject);
public:
	JYObject(cocos2d::CCNode* pCCObject);
	virtual ~JYObject();

	JYOBJECT_TYPE getObjectType() { return m_nObjectType; }
	void setObjectType(const JYOBJECT_TYPE& nType) { m_nObjectType = nType; }
	UINT getUID() { return m_nUID; }
	void setUID(const UINT& nUID) { m_nUID = nUID; }
	UINT getTag() { return m_nTag; }
	void setTag(const UINT& nTag) { m_nTag = nTag; }
	float getVelocity() { return m_fVelocity; }
	void setVelocity(const float& fVelocity) { m_fVelocity = fVelocity; }
	float getDirection() { return m_fDirection; }
	void setDirection(const float& fDirection) { m_fDirection = fDirection; }
	BaseExecuter* getExecuter(int type) { return m_Executers[type]; }
	void setExecuter(BaseExecuter* param);
	void ReleaseExecuter(__Executer _eType);

	cocos2d::CCNode* getCCObject() { return m_pCCObject; }
	void setCCObject(cocos2d::CCNode* ccOb) { m_pCCObject = ccOb; }

	void setName(const std::string& sName) { m_sName = sName; }
	std::string getName() { return m_sName; }
	JYObject* getParent() { return m_pJYObjectParent; }
	void setParent(JYObject* pJYObject) { m_pJYObjectParent = pJYObject; }
	void addChild(JYObject* const pJYObject);
	JYObject* getChildByName(const std::string& sName);
	JYObject* getChildByTag(const UINT& nTag);
	void popChildByName(const std::string& sName);
	void popChildByJYObject(JYObject* const pJYObject);
	
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual	void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime);
};