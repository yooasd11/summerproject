#ifndef __MyScene_SCENE_H__
#define __MyScene_SCENE_H__

#include "cocos2d.h"

class JYRealTimer;

USING_NS_CC;

class JYObject;
class JYPlayer;
class JYArm;

class MyScene : public cocos2d::Layer
{
private:
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	UINT nPlayerUID;
	CCSprite* pFireAim;
	JYArm * pJYArmBullet;
	JYPlayer* pJYPlayerDragon;
	CCParallaxNode* backgroundNode;
	CCSize winSize;
	JYRealTimer* pTimer;

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
	virtual void onMouseMove(Event* pEvent);
	virtual void onMouseDown(Event* pEvent);
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent);
	void addBackground();
	void createDragon(const UINT& nUID, const CCPoint& pos);
	void deleteDragon(const UINT& nUID);
	void makePlayer(JYObject* const pJYPlayer);
	void setViewPointCenter(CCPoint pos);
	void callEveryFrame(float fDeltaTime);
	// implement the "static create()" method manually
	CREATE_FUNC(MyScene);
};

#endif // __MyScene_SCENE_H__
