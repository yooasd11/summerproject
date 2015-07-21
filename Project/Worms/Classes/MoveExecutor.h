#pragma once
#include "cocos2d.h"
#include "BaseExecutor.h"

class MoveExecutor : public BaseExecutor{
	GENERATE_FUNC(MoveExecutor)
private:
	int nActionTagForKeycode[(int)(cocos2d::EventKeyboard::KeyCode::KEY_PLAY) + 1];

	void handleKeycodeMovement(cocos2d::EventKeyboard::KeyCode keyCode);
	cocos2d::CCPoint getNextPos(cocos2d::CCPoint pos, cocos2d::EventKeyboard::KeyCode keyCode);
	cocos2d::CCPoint tileCoordForPostion(cocos2d::CCPoint pos);
	
	void setPlayerPosition(cocos2d::CCPoint src, cocos2d::CCPoint dst);
	bool boundaryCollisionChecker(cocos2d::CCPoint& src, cocos2d::CCPoint& dst);
	bool objectCollisionChecker(cocos2d::CCPoint& src, cocos2d::CCPoint& dst);
public:
	MoveExecutor(JYObject* pOwner) : BaseExecutor(pOwner) {
		regist();
		memset(nActionTagForKeycode, 0, sizeof(nActionTagForKeycode));
	}
	virtual ~MoveExecutor() {}
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
	virtual void tick(float fDeltaTime);
};

