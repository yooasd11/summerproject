#pragma once
#include "JYObject.h"

class JYArm : public JYObject{
private:
	float m_fDamage;
public:
	JYArm(cocos2d::CCNode* pCCObject);
	virtual ~JYArm() {}

	void setDamage(const float& fDamage){
		m_fDamage = fDamage;
	}

	float getDamage(){
		return m_fDamage;
	}
};