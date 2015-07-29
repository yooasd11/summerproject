#pragma once

#include <functional>

class JYLocalTimer{
private:
	float m_fAlarmTime;
	float m_fPassedTime;
	std::function<bool()> m_alarmFunc;
public:
	JYLocalTimer(const float& fAlarmTime) : m_fAlarmTime(fAlarmTime), m_fPassedTime(0.0f) {}
	JYLocalTimer(const float& fAlarmTime, std::function<bool()>& pFunc) : JYLocalTimer(fAlarmTime) { setAlarmFunc(pFunc); }

	void addTime(const float& fDeltaTime) { this->m_fPassedTime += fDeltaTime; }
	bool isDeadlined() { return m_fPassedTime >= m_fAlarmTime; }

	void setAlarmFunc(std::function<bool()>& pFunc){
		this->m_alarmFunc = pFunc;
	}

	void alarm(){ 
		this->m_fPassedTime = 0.0f; 
		if (this->m_alarmFunc() == true) {}
	}
};