#include "AccelerationExecuter.h"
#include "JYObject.h"

void AccelerationExecuter::addAcceleration(const float& fAccX, const float& fAccY){
	Acceleration newAcc(fAccX, fAccY);
	this->m_vecAccList.push_back(newAcc);
}

void AccelerationExecuter::executeAcceleration(Acceleration& const acc, const float& fDeltaTime){
	JYObject* pOwner = this->getOwner();
	if (pOwner == nullptr) return;
	cocos2d::CCNode* pCCOwner = pOwner->getCCObject();
	if (pCCOwner == nullptr) return;

	float fAccX = acc.getAccelerationX();
	float fAccY = acc.getAccelerationY();

	float fNextVx = pOwner->getVelocityX() + fAccX *fDeltaTime;
	float fNextVy = pOwner->getVelocityY() + fAccY *fDeltaTime;

	pOwner->setVelocity(fNextVx, fNextVy);
}

void AccelerationExecuter::tick(float fDeltaTime){
	for (auto it = this->m_vecAccList.begin(); it != this->m_vecAccList.end();){
		this->executeAcceleration(*it, fDeltaTime);
		++it;
	}
}

