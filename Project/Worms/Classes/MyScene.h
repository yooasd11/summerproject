#ifndef __MyScene_SCENE_H__
#define __MyScene_SCENE_H__

#include "cocos2d.h"
#include "InGamePacket.pb.h"
#include "AccountPacket.pb.h"

USING_NS_CC;

#define GET_MYSCENE (MyScene*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene")
//#define GET_TMAP (cocos2d::CCTMXTiledMap*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene")->getChildByName("Background")->getChildByName("Tmap")

#define GET_TMAP (cocos2d::CCTMXTiledMap*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene")->getChildByName("Tmap")

class JYObject;
class JYPlayer;
class JYArm;
class JYRealTimer;

class MyScene : public cocos2d::Layer
{
private:
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	UINT nPlayerUID;
	JYPlayer* pJYPlayerDragon;
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
	JYObject* createBullet(const InGamePacket::S_Shoot& sShootPacket);
	JYObject* createDragon(const UINT& nUID, const UINT& nHP, const float& fX, const float& fY);
	void deleteDragon(const UINT& nUID);
	void makePlayer(JYObject* const pJYPlayer);
	void setViewPointCenter(CCPoint pos);
	void callEveryFrame(float fDeltaTime);
	// implement the "static create()" method manually
	CREATE_FUNC(MyScene);
};

#endif // __MyScene_SCENE_H__
