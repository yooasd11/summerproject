#include "BaseExecutor.h"

class AssaultExecutor : public BaseExecutor{
	GENERATE_FUNC(AssaultExecutor)
private:
public:
	AssaultExecutor(JYObject* pJYObject) : BaseExecutor(pJYObject){
		regist();
	};
	virtual ~AssaultExecutor() {}
	virtual void onMouseMove(cocos2d::Event* pEvent);
	virtual void onMouseDown(cocos2d::Event* pEvent);
};