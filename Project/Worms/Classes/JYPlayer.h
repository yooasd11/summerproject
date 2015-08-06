#pragma once
#include "JYObject.h"

class JYPlayer : public JYObject{
private:
	cocos2d::CCProgressTimer* m_pCCRespawnTimer;
	UINT m_nHP;
	void createRespawnTimer();
	void executeRespawnTimer(const float& fSec);
public:
	JYPlayer(cocos2d::CCNode* pCCObject);

	void setHP(const UINT& nHP);
	UINT getHP() { return m_nHP; }

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual	void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
	virtual void onMouseUp(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime){
		JYObject::tick(fDeltaTime);
	}
	bool respawnDeltaTimeFunc(const float& fDeltaTime);
	bool respawnCallBackFunc();
};