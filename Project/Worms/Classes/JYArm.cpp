#include "JYArm.h"
#include "CollisionExecutor.h"

JYArm::JYArm(cocos2d::CCNode* pCCObject) : JYObject(pCCObject) {
	new CollisionExecutor(this);
}