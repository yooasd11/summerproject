#include "BaseExecuter.h"

class AssaultExecuter : public BaseExecuter{
	GENERATE_FUNC(AssaultExecuter)
private:
	UINT m_nBulletCounter;
public:
	AssaultExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject, __AssaultExecuter){
		regist();
		m_nBulletCounter++;
	};
	virtual ~AssaultExecuter() {}
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
};