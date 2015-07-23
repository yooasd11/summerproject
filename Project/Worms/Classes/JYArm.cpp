#include "JYArm.h"
#include "MoveExecutor.h"
#include "CollisionExecutor.h"

JYArm::JYArm(cocos2d::CCNode* pCCObject) : JYObject(pCCObject) {
	new CollisionExecutor(this);
	new MoveExecutor(this);
	this->setDirection(0.0f);
	this->setObjectType(JYOBJECT_TYPE::JY_ARM);
}
