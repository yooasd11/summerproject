#include "BaseExecuter.h"
#include "JYObject.h"
#include "JYLocalTimer.h"

class JYLocalTimer;

class AssaultExecuter : public BaseExecuter{
	GENERATE_FUNC(AssaultExecuter)
private:
	cocos2d::CCProgressTimer* m_pCCCoolTimer;
	bool m_bIsCooling;
	bool m_bIsCharging;
	float m_fDirection;
	void createCoolTimer();
	void executeCoolTimer(float fSec);
public:
	AssaultExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject, __Executer::__AssaultExecuter){
		regist();
		this->m_bIsCooling = false;
		this->m_bIsCharging = false;
		this->createCoolTimer();
	};

	virtual ~AssaultExecuter() {}
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
	virtual void onMouseUp(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime);
	bool coolTimeCallBackFunc();
	bool coolTimeDeltaFunc(const float& fDeltaTime);
};