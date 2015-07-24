#pragma once

#include "ExecuterList.h"
#include "cocos2d.h"

enum __Executer;
class JYObject;

class BaseExecuter{
//	public:
//	virtual char* getName() = 0;
//	virtual __Executer getEnum() = 0;
private:
	JYObject* m_pOwner;
	__Executer m_eType;
public:
	BaseExecuter(JYObject* pOwner, __Executer eType) : m_pOwner(pOwner), m_eType(eType) {}
	__Executer getEnum() { return m_eType; }
	virtual ~BaseExecuter(); 
	void regist();

	JYObject* const getOwner() const { return m_pOwner; }
	void setOwner(JYObject* pOwner) {m_pOwner = pOwner; }
	virtual void tick(float fDeltaTime);
};

#define GENERATE_FUNC(PARAM) \
	public: \
	static char* getName() { return #PARAM; } \
	private: 