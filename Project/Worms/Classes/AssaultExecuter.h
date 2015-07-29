#include "BaseExecuter.h"
#include "JYObject.h"

class AssaultExecuter : public BaseExecuter{
	GENERATE_FUNC(AssaultExecuter)
private:
	cocos2d::CCSprite* m_pCCSpriteCoolTimer;
	cocos2d::CCAnimate*	m_pCCAnimateCoolTimer;
	bool m_bIsCooling;
	void createCoolTimer();
	void executeCoolTimer();
public:
	AssaultExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject, __AssaultExecuter){
		regist();
		m_bIsCooling = false;

		this->createCoolTimer();
		this->getOwner()->getCCObject()->addChild(this->m_pCCSpriteCoolTimer);
		this->m_pCCSpriteCoolTimer->setVisible(false);
	};

	virtual ~AssaultExecuter() {}
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
};