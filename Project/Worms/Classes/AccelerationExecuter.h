#pragma once
#include "BaseExecuter.h"

class AccelerationExecuter : public BaseExecuter{
	GENERATE_FUNC(AccelerationExecuter)
private:
	float m_fAccX;
	float m_fAccY;
	/*class Acceleration{
	private:
		float m_fAx;
		float m_fAy;
	public:
		Acceleration() {}
		Acceleration(const float& fAccX, const float& fAccY){
			this->setAccelerationX(fAccX);
			this->setAccelerationY(fAccY);
		}

		void setAccelerationX(const float& fAcceleration) { this->m_fAx = fAcceleration; }
		void setAccelerationY(const float& fAcceleration) { this->m_fAy = fAcceleration; }
		float getAccelerationX() { return this->m_fAx; }
		float getAccelerationY() { return this->m_fAy; }
	};

	std::vector<Acceleration> m_vecAccList;*/
public:
	AccelerationExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject, __Executer::__AccelerationExecuter){
		regist();
	}

	virtual ~AccelerationExecuter() {}

	void addAcceleration(const float& fAccX, const float& fAccY);
	void executeAcceleration(const float& fDeltaTime);
	virtual void tick(float fDeltaTime);
};