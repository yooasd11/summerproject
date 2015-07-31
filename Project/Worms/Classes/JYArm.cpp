#include "JYArm.h"
#include "MoveExecuter.h"
#include "CollisionExecuter.h"
#include "AccelerationExecuter.h"

JYArm::JYArm(cocos2d::CCNode* pCCObject) : JYObject(pCCObject) {
	new CollisionExecuter(this);
	new MoveExecuter(this);
	//new AccelerationExecuter(this);
	this->setObjectType(JYOBJECT_TYPE::JY_ARM);
}
