#include "JYArm.h"
#include "MoveExecuter.h"
#include "CollisionExecuter.h"

JYArm::JYArm(cocos2d::CCNode* pCCObject) : JYObject(pCCObject) {
	new CollisionExecuter(this);
	new MoveExecuter(this);
	this->setDirection(0.0f);
	this->setObjectType(JYOBJECT_TYPE::JY_ARM);
}
