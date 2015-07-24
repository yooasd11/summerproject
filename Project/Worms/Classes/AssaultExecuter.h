#include "BaseExecuter.h"

class AssaultExecuter : public BaseExecuter{
	GENERATE_FUNC(AssaultExecuter)
private:
	int m_nBulletCounter;
	void createBullet();
public:
	AssaultExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject, __AssaultExecuter){
		regist();
		m_nBulletCounter = 0;
	};
	virtual ~AssaultExecuter() {}
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
};