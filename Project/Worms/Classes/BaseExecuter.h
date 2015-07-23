#pragma once

#include "ExecuterList.h"
#include "cocos2d.h"

enum __Executer;
class JYObject;

class BaseExecuter{
public:
	virtual char* getName() = 0;
	virtual __Executer getEnum() = 0;
private:
	JYObject* m_pOwner;
public:
	BaseExecuter(JYObject* pOwner) : m_pOwner(pOwner) {}
	virtual ~BaseExecuter(); 
	void regist();

	JYObject* const getOwner() const { return m_pOwner; }
	void setOwner(JYObject* pOwner) {m_pOwner = pOwner; }
	virtual void tick(float fDeltaTime);
};

#define GENERATE_FUNC(PARAM) \
	public: \
	virtual char* getName() override { return #PARAM; } \
	virtual __Executer getEnum() override { return __##PARAM; } \
	private: 