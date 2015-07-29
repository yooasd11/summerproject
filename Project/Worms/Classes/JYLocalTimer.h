#pragma once

#include <functional>

class JYLocalTimer{
	//inner class
private:
	class JYTimer{
		private:
			float m_fAlarmTime;
			float m_fPassedTime;
			std::function<bool()> m_callBackFunc;
			std::function<bool(const float&)> m_deltaTimeFunc;
		public:
			JYTimer(const float& fAlarmTime) : m_fAlarmTime(fAlarmTime), m_fPassedTime(0.0f) {}
			JYTimer(const float& fAlarmTime, std::function<bool()>&& callBackFunc) : JYTimer(fAlarmTime) { setCallBackFunc(callBackFunc); }
			JYTimer(const float& fAlarmTime, std::function<bool()>&& callBackFunc, std::function<bool(const float&)>&& deltaTimeFunc) : JYTimer(fAlarmTime, std::move(callBackFunc)){
				setDeltaTimeFunc(deltaTimeFunc);
			}

			void tick(const float& fDeltaTime) { 
				this->m_fPassedTime += fDeltaTime; 
				if (this->m_deltaTimeFunc(fDeltaTime) == true) {}
			}
			bool isDeadlined() { return m_fPassedTime >= m_fAlarmTime; }

			void setCallBackFunc(std::function<bool()>& pFunc){
				this->m_callBackFunc = pFunc;
			}

			void setDeltaTimeFunc(std::function<bool(const float&)>& pFunc){
				this->m_deltaTimeFunc = pFunc;
			}

			void alarm(){
				this->m_fPassedTime = 0.0f;
				if (this->m_callBackFunc() == true) {}
			}
	};
	std::vector< std::pair<UINT, JYTimer> > m_TimerList;
	UINT m_nTimerIDGenerated;
public:
	JYLocalTimer() : m_nTimerIDGenerated(0) {}
	
	void pushTimer(const float& fAlarmTime, std::function<bool()>&& callBackFunc, std::function<bool(const float&)>&& deltaTimeFunc = nullptr){
		JYTimer newTimer(fAlarmTime, std::move(callBackFunc), std::move(deltaTimeFunc));
		m_TimerList.push_back(std::make_pair(m_nTimerIDGenerated++, newTimer));
	}

	void tick(const float& fDeltaTime){
		for (auto it = m_TimerList.begin(); it != m_TimerList.end();){
			JYTimer& curTimer = (*it).second;
			curTimer.tick(fDeltaTime);
			if (curTimer.isDeadlined() == true){
				curTimer.alarm();
				it = m_TimerList.erase(it);
			}
			else
				it++;
		}
	}
};