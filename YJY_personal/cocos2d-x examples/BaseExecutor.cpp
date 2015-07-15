#include "BaseExecutor.h"
#include "JYObject.h"
#include "cocos2d.h"

BaseExecutor::BaseExecutor(JYObject* pOwner) : m_pOwner(pOwner)
{
}

BaseExecutor::~BaseExecutor()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->ReleaseExecutor(this);
	}
}

void BaseExecutor::regist()
{
	m_pOwner->setExecutor(this);
}

JYObject* const BaseExecutor::getOwner() const{
	return m_pOwner;
}

void BaseExecutor::setOwner(JYObject* pOwner){
	m_pOwner = pOwner;
	return;
}

void BaseExecutor::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
}

void BaseExecutor::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent) {
}

void BaseExecutor::tick(float fDeltaTime){
}

void BaseExecutor::onMouseMove(cocos2d::Event* pEvent){

}