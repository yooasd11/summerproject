#include "AccelerationExecuter.h"
#include "JYObject.h"

void AccelerationExecuter::addAcceleration(const float& fAccX, const float& fAccY){
	this->m_fAccX += fAccX;
	this->m_fAccY += fAccY;
}

void AccelerationExecuter::setAcceleration(const float& fAccX, const float& fAccY){
	this->m_fAccX = fAccX;
	this->m_fAccY = fAccY;
}

void AccelerationExecuter::executeAcceleration(const float& fDeltaTime){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCNode* pCCOwner = pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	float fNextVx = pOwner->getVelocityX() + this->m_fAccX *fDeltaTime;
	float fNextVy = pOwner->getVelocityY() + this->m_fAccY *fDeltaTime;

	pOwner->setVelocity(fNextVx, fNextVy);
}

void AccelerationExecuter::clearAcceleration(){
	this->m_fAccX = 0.0f;
	this->m_fAccY = 0.0f;
}

void AccelerationExecuter::tick(float fDeltaTime){
	this->executeAcceleration(fDeltaTime);
}

