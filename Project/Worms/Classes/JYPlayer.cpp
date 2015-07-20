#include "JYPlayer.h"
#include "MoveExecutor.h"
#include "CollisionExecutor.h"
#include "CoordinateConverter.h"
#include "Packet.h"
#include "Inputhandler.h"

JYPlayer::JYPlayer(cocos2d::CCNode* pCCObject) : JYObject(pCCObject){
	new MoveExecutor(this);
	m_nUID = -1;
}
void JYPlayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	MoveExecutor* pMoveExecutor = (MoveExecutor*)this->getExecutor(__MoveExecutor);
	if (pMoveExecutor != nullptr)
	{
		pMoveExecutor->onKeyPressed(keyCode, pEvent);
	}
}

void JYPlayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	MoveExecutor* pMoveExecutor = (MoveExecutor*)this->getExecutor(__MoveExecutor);
	if (pMoveExecutor != nullptr)
	{
		pMoveExecutor->onKeyReleased(keyCode, pEvent);
	}
}

void JYPlayer::onMouseMove(cocos2d::Event* pEvent){
	MoveExecutor* pMoveExecutor = (MoveExecutor*)this->getExecutor(__MoveExecutor);
	if (pMoveExecutor != nullptr)
	{
		pMoveExecutor->onMouseMove(pEvent);
	}
}

void JYPlayer::onMouseDown(cocos2d::Event* pEvent){
	cocos2d::CCLayer* pNowScene = (cocos2d::CCLayer*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
	cocos2d::CCSprite* pBullet = (cocos2d::CCSprite*)pNowScene->getChildByName("Bullet");
	this->shootBullet(pBullet, pEvent);
}

void JYPlayer::shootBullet(cocos2d::CCNode* pBullet, cocos2d::Event* pEvent){
	cocos2d::CCSprite* pDragon = (cocos2d::CCSprite*)this->getCCObject();
	cocos2d::CCSprite* pAim = (cocos2d::CCSprite*)pDragon->getChildByName("Aim");

	cocos2d::CCPoint bulletPos = pAim->convertToWorldSpace(pAim->getPosition());
	pBullet->setPosition(bulletPos);
	pBullet->setRotation(CoordinateConverter::getInstance()->getDegreeBetweenCCNodeAndMouse(pBullet, pEvent));
	pBullet->setVisible(true);
	CCLOG("Bullet position (%.2f, %.2f)", pBullet->getPosition().x, pBullet->getPosition().y);

	cocos2d::CCPoint mouseWorldPos = CoordinateConverter::getInstance()->getMouseGlobalPos(pEvent);
	cocos2d::CCPoint mouseNodePos = cocos2d::CCDirector::getInstance()->getRunningScene()->convertToNodeSpace(mouseWorldPos);
	cocos2d::CCAction* fireBullet = cocos2d::CCMoveTo::create(0.5f, mouseNodePos);
	CCLOG("Shoot to world (%.2f, %.2f)", mouseWorldPos.x, mouseWorldPos.y);

	pBullet->runAction(fireBullet);
}