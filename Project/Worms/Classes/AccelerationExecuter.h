#pragma once
#include "BaseExecuter.h"

class AccelerationExecuter : public BaseExecuter{
	GENERATE_FUNC(AccelerationExecuter)
private:
	class Acceleration{
	private:
		float m_fDirection;
		float m_fAcceleration;
	public:
		Acceleration() {}
		Acceleration(const float& fDirection, const float& fAcceleration){
			setDirection(fDirection);
			setAcceleration(fAcceleration);
		}
		void setDirection(const float& fDirection) { m_fDirection = fDirection; }
		void setAcceleration(const float& fAcceleration) { m_fAcceleration = fAcceleration; }
		float getDirection() { return m_fDirection; }
		float getAcceleration() { return m_fAcceleration; }
	};

	std::vector<Acceleration> m_vecAccList;
public:
	AccelerationExecuter(JYObject* pJYObject) : BaseExecuter(pJYObject, __Executer::__AccelerationExecuter){
		regist();
		addGravity();
	}

	virtual ~AccelerationExecuter() {}

	void addGravity();
	void addAccerlation(const float& fDirection, const float& fAcceleration);
	void executeGravity(Acceleration& const acc, const float& fDeltaTime);
	virtual void tick(float fDeltaTime);
};