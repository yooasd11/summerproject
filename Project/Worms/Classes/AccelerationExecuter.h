#pragma once
#include "BaseExecuter.h"

class AccelerationExecuter : public BaseExecuter{
	GENERATE_FUNC(AccelerationExecuter)
private:
	float m_fAccX;
	float m_fAccY;
public:
	AccelerationExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject, __Executer::__AccelerationExecuter){
		regist();
		this->m_fAccX = 0.0f;
		this->m_fAccY = 0.0f;
	}

	virtual ~AccelerationExecuter() {}

	void addAcceleration(const float& fAccX, const float& fAccY);
	void setAcceleration(const float& fAccX, const float& fAccY);
	void executeAcceleration(const float& fDeltaTime);
	void clearAcceleration();
	virtual void tick(float fDeltaTime);
};