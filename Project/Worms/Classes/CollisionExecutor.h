#pragma once

#include "BaseExecutor.h"

class JYObject;

class CollisionExecutor : public BaseExecutor{
	GENERATE_FUNC(CollisionExecutor)
private:
	cocos2d::CCPoint getNextPos(cocos2d::CCPoint currentPos, float fDeltaTime);
	cocos2d::CCPoint tileCoordForPostion(cocos2d::CCPoint pos);
	bool boundaryCollisionChecker(cocos2d::CCPoint& dst);
	bool objectCollisionChecker(cocos2d::CCPoint& dst);
public:
	CollisionExecutor(JYObject* pJYObject) : BaseExecutor(pJYObject){
		regist();
	}
	virtual ~CollisionExecutor() {}
	virtual void tick(float fDeltaTime);
};