#include "AccelerationExecuter.h"
#include "JYObject.h"

void AccelerationExecuter::addGravity(){
	this->addAccerlation(180.0f, 10.0f);
}

void AccelerationExecuter::addAccerlation(const float& fDirection, const float& fAcceleration){
	Acceleration newAcc(fDirection, fAcceleration);
	this->m_vecAccList.push_back(newAcc);
}

void AccelerationExecuter::executeGravity(Acceleration& const acc, const float& fDeltaTime){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCNode* pCCOwner = pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	float fAcceleration = acc.getAcceleration();
	float fAccDirection = acc.getDirection();
	float fAccRadian = MATH_DEG_TO_RAD(180.0f);

	float sinDegree = sin(fAccRadian);

	float fAccX = fAcceleration * sin(fAccRadian);
	float fAccY = fAcceleration * cos(fAccRadian);

	float fVelocity = pOwner->getVelocity();
	float fVelDirection = pOwner->getDirection();
	float fVelRadian = MATH_DEG_TO_RAD(fVelDirection);

	float fVX = fVelocity * sin(fVelRadian) + fAccX * fDeltaTime;
	float fVY = fVelocity * cos(fVelRadian) + fAccY * fDeltaTime;

	float fDX = fVX * fDeltaTime + 0.5f * fAccX * fDeltaTime * fDeltaTime;
	float fDY = fVY * fDeltaTime + 0.5f * fAccY * fDeltaTime * fDeltaTime;

	cocos2d::CCPoint newPos = pCCOwner->getPosition() + cocos2d::ccp(fDX, fDY);
	pCCOwner->setPosition(newPos);

	pOwner->setVelocity(fVX / sin(fVelRadian));
}

void AccelerationExecuter::tick(float fDeltaTime){
	for (auto it = this->m_vecAccList.begin(); it != this->m_vecAccList.end();){
		this->executeGravity(*it, fDeltaTime);
	}
}

