#include "BaseExecuter.h"
#include "JYObject.h"
#include "JYLocalTimer.h"

class JYLocalTimer;

class AssaultExecuter : public BaseExecuter{
	GENERATE_FUNC(AssaultExecuter)
private:
	std::vector<JYLocalTimer> m_vecTimers;
	cocos2d::CCSprite* m_pCCSpriteCoolTimer;
	bool m_bIsCooling;
	void createCoolTimer();
	void executeCoolTimer();
public:
	AssaultExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject, __AssaultExecuter){
		regist();
		m_bIsCooling = false;

		this->createCoolTimer();
	};

	virtual ~AssaultExecuter() {}
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime);
	bool coolTimeAlarmFunc();
};