#pragma once

#include "JYObject.h"

class JYItem : public JYObject{
private:
public:
	JYItem(cocos2d::CCNode* pCCObject) : JYObject(pCCObject) {}
	virtual ~JYItem() {}
};