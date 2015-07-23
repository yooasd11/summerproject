#pragma once

#include "BaseExecuter.h"

class JYObject;

class CollisionExecuter : public BaseExecuter{
	GENERATE_FUNC(CollisionExecuter)
private:
	cocos2d::CCPoint getNextPos(cocos2d::CCPoint currentPos, float fDeltaTime);
	cocos2d::CCPoint tileCoordForPostion(cocos2d::CCPoint pos);
	bool boundaryCollisionChecker(cocos2d::CCPoint& dst);
	bool objectCollisionChecker(cocos2d::CCPoint& dst);
public:
	CollisionExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject){
		regist();
	}
	virtual ~CollisionExecuter() {}
	virtual void tick(float fDeltaTime);
};