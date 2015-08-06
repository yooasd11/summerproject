#pragma once

#include "ExecuterList.h"
#include "cocos2d.h"
#include "JYLocalTimer.h"

enum __Executer;
class JYObject;

class BaseExecuter{
private:
	JYLocalTimer m_JYLocalTimer;
	JYObject* m_pOwner;
	__Executer m_eType;
public:
	BaseExecuter(JYObject* pOwner, __Executer eType) : m_pOwner(pOwner), m_eType(eType) {
	}
	__Executer getEnum() { return m_eType; }
	virtual ~BaseExecuter(); 
	void regist();

	JYLocalTimer* getLocalTimer() { return &this->m_JYLocalTimer; }
	JYObject* const getOwner() const { return m_pOwner; }
	void setOwner(JYObject* pOwner) {m_pOwner = pOwner; }
	virtual void tick(float fDeltaTime);
};

#define GENERATE_FUNC(PARAM) \
	public: \
	static char* getName() { return #PARAM; } \
	private: 