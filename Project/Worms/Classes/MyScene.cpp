#include "MyScene.h"
#include "MultiByteConverter.h"
#include "XMLParser.h"
#include "JYObject.h"
#include "JYPlayer.h"
#include "JYItem.h"
#include "JYArm.h"
#include "MoveExecuter.h"
#include "Inputhandler.h"
#include "JYRealTimer.h"
#include "ConnectionManager.h"
#include "AccountPacket.pb.h"
#include "InGamePacket.pb.h"
#include "Packet.h"
#include "JYObjectManager.h"
#include "CoordinateConverter.h"
#include "AccelerationExecuter.h"

//#define SERVER_IP_ADDRESS "10.1.4.85"
#define SERVER_IP_ADDRESS "localhost"
#define SERVER_PORT_NUMBER 9200

Scene* MyScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MyScene::create();
	layer->setName("MyScene");
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void MyScene::onEnter(){
	CCLayer::onEnter();

	ConnectionManager::getInstance()->accountTo(SERVER_IP_ADDRESS, SERVER_PORT_NUMBER);
	ConnectionManager::getInstance()->receive();

	EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(MyScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(MyScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MyScene::onTouchEnded, this);

	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(InputHandler::onKeyPressed, InputHandler::getInstance());
	keyListener->onKeyReleased = CC_CALLBACK_2(InputHandler::onKeyReleased, InputHandler::getInstance());

	EventListenerMouse* mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(InputHandler::onMouseMove, InputHandler::getInstance());
	mouseListener->onMouseDown = CC_CALLBACK_1(InputHandler::onMouseDown, InputHandler::getInstance());
	mouseListener->onMouseUp = CC_CALLBACK_1(InputHandler::onMouseUp, InputHandler::getInstance());
	mouseListener->onMouseScroll = CC_CALLBACK_1(InputHandler::onMouseScroll, InputHandler::getInstance());

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void MyScene::onExit(){
	this->unschedule(schedule_selector(MyScene::callEveryFrame));
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	_eventDispatcher->removeEventListenersForType(EventListener::Type::KEYBOARD);
	_eventDispatcher->removeEventListenersForType(EventListener::Type::MOUSE);
	CCLayer::onExit();
}

// on "init" you need to initialize your instance
bool MyScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->pJYPlayerDragon = nullptr;
	winSize = CCDirector::getInstance()->getWinSize();
	CCLOG("WinSize : (%.2f, %.2f)", winSize.width, winSize.height);
	this->addBackground();

	pTimer = new JYRealTimer();
	this->schedule(schedule_selector(MyScene::callEveryFrame));
	return true;
}

void MyScene::addBackground(){
	//map currently working on
	CCTMXTiledMap* pTmap = CCTMXTiledMap::create("TileMaps/TestMap.tmx");

	pTmap->setName("Tmap");
	/*pBackgroundNode = CCParallaxNode::create();
	pBackgroundNode->setName("Background");
	pBackgroundNode->addChild(pTmap, 1, ccp(1.0f, 1.0f), ccp(0, 0));
	this->addChild(pBackgroundNode, 0);*/
	this->addChild(pTmap);
	CCLOG("TMAP size : (%.1f, %.1f)", pTmap->getContentSize().width, pTmap->getContentSize().height);
}

JYObject* MyScene::createDragon(const AccountPacket::S_Account_List::Account& sAccountPacket){
	UINT nUID = sAccountPacket.uid();
	UINT nHP = sAccountPacket.hp();
	float fX = sAccountPacket.x();
	float fY = sAccountPacket.y();

	JYPlayer* pJYPlayer = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pJYPlayer != nullptr) return nullptr;		//already created

	CCTexture2D* texture = CCDirector::getInstance()->getTextureCache()->addImage("Animations/dragon_animation.png");
	CCAnimation* animation = Animation::create();
	animation->setDelayPerUnit(0.05f);
	for (int i = 0; i < 6; ++i){
		int index = 1 % 4;
		int rowIndex = i / 4;
		animation->addSpriteFrameWithTexture(texture, Rect(index * 130, rowIndex * 140, 130, 140));
	}

	//Create CCSprite dragon, and add to Tiled map
	CCSprite* pDragon = CCSprite::createWithTexture(texture, Rect(0, 0, 130, 140));
	pDragon->setPosition(ccp(fX,fY));
	pDragon->setFlippedX(true);
	pDragon->setScale(0.5);
	pDragon->setName("Dragon");
	CCTMXTiledMap* pTmap = GET_TMAP;
	pTmap->addChild(pDragon, 3);

	//Run action : flying animation
	Animate* animate = Animate::create(animation);
	RepeatForever* rep = RepeatForever::create(animate);
	pDragon->runAction(rep);

	CCSprite* pHPBar = CCSprite::create("HPBar.png");
	CCProgressTimer* pProgressHPBar = CCProgressTimer::create(pHPBar);
	pProgressHPBar->setType(CCProgressTimer::Type::BAR);
	pProgressHPBar->setPercentage(100.0f);
	pProgressHPBar->setMidpoint(ccp(0.0f, 0.0f));
	pProgressHPBar->setBarChangeRate(ccp(1, 0));
	pProgressHPBar->setName("HPBar");
	pProgressHPBar->setPosition(ccp(pDragon->getContentSize().width / 2, pDragon->getContentSize().height / 2));
	pProgressHPBar->setScale(0.6f);
	pDragon->addChild(pProgressHPBar);

	//Create JYPlayer with CCSprite dragon
	pJYPlayer = new JYPlayer(pDragon);
	pJYPlayer->setUID(nUID);
	pJYPlayer->setHP(nHP);


	if (nUID == this->nPlayerUID){
		this->makePlayer(pJYPlayer);
	}
	return pJYPlayer;
}

JYObject* MyScene::createBullet(const InGamePacket::S_Shoot& sShootPacket){
	UINT nShooterUID = sShootPacket.uid();
	UINT nBulletUID = sShootPacket.bullet_uid();
	float fX = sShootPacket.x();
	float fY = sShootPacket.y();
	float fVx = sShootPacket.vx();
	float fVy = sShootPacket.vy();
	float fDamage = sShootPacket.damage();

	JYPlayer* pJYPlayer = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nShooterUID);
	if (pJYPlayer == nullptr) return nullptr;
	JYArm* pJYBullet = (JYArm*)JYObjectManager::getInstance()->findObjectByUID(nBulletUID);
	if (pJYBullet != nullptr) return nullptr;				//bullet already created
	CCSprite* pCCOwner = (cocos2d::CCSprite*)pJYPlayer->getCCObject();
	if (pCCOwner == nullptr) return nullptr;
	CCTMXTiledMap* pTmap = GET_TMAP;
	if (pTmap == nullptr) return nullptr;

	//Create CCSprite bullet
	CCSprite* bullet = CCSprite::create("bullet.png");
	bullet->setScale(0.2f);
	bullet->setPosition(ccp(fX,fY));

	//Create JYArm with CCSprite bullet and add
	JYArm* pJYArmBullet = new JYArm(bullet);
	pJYPlayer->addChild(pJYArmBullet);
	pJYArmBullet->setObjectType(JYOBJECT_TYPE::JY_ARM);
	pJYArmBullet->setUID(nBulletUID);
	pJYArmBullet->setVelocity(fVx, fVy);
	pJYArmBullet->setDamage(fDamage);
	pTmap->addChild(bullet);

	AccelerationExecuter* pAccExecuter = (AccelerationExecuter*)pJYArmBullet->getExecuter(__Executer::__AccelerationExecuter);
	if (pAccExecuter == nullptr){
		CCLOG("No acceleration executer");
		return pJYArmBullet;
	}

	int nAccCounter = sShootPacket.acceleration_list_size();
	for (int i = 0; i < nAccCounter; ++i){
		InGamePacket::S_Acceleration sAcc;
		sAcc = sShootPacket.acceleration_list(i);
		
		float fAccX = sAcc.ax();
		float fAccY = sAcc.ay();

		pAccExecuter->addAcceleration(fAccX, fAccY);
	}

	return pJYArmBullet;
}

//Make pJYPlayer as controllable player of the scene
void MyScene::makePlayer(JYObject* const pJYPlayer){
	CCNode* pCCNode = pJYPlayer->getCCObject();

	CCSprite* pCharger = CCSprite::create("charge.png");
	CCProgressTimer* pProgressCharger = CCProgressTimer::create(pCharger);
	pProgressCharger->setType(CCProgressTimer::Type::BAR);
	pProgressCharger->setPercentage(0.0f);
	pProgressCharger->setMidpoint(ccp(0.0f, 0.0f));
	pProgressCharger->setBarChangeRate(ccp(1.0f, 0.0f));
	pProgressCharger->setName("Charger");
	pProgressCharger->setPosition(ccp(pCCNode->getContentSize().width / 2, 0));
	pProgressCharger->setVisible(false);
	pCCNode->addChild(pProgressCharger);

	//make aim
	CCSprite* pFireAim = CCSprite::create("line.png");
	pFireAim->setPosition(ccp(pCCNode->getContentSize().width/2, pCCNode->getContentSize().height/3)); //position value obtained through experiment
	pFireAim->setName("Aim");
	pFireAim->setAnchorPoint(ccp(0.5f, -0.2f));
	pCCNode->addChild(pFireAim);


	pJYPlayerDragon = (JYPlayer*)pJYPlayer;
}


bool MyScene::onTouchBegan(Touch* pTouch, Event* pEvent){
	return true;
}

void MyScene::onTouchMoved(Touch* pTouch, Event* pEvent){
	if (InputHandler::getInstance()->isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL) == false) return;
	MyScene* pMyScene = GET_MYSCENE;
	CCTMXTiledMap* pTmap = GET_TMAP;

	CCSize winViewSize = CCDirector::getInstance()->getOpenGLView()->getFrameSize();
	CCSize tmapSize = pTmap->getContentSize();
	CCPoint mouseDeltaPos = pTouch->getDelta();
	CCPoint newPos = pTmap->getPosition() + pTouch->getDelta();
	if (newPos.x > 0) newPos.x = 0;
	if (winViewSize.width - tmapSize.width >= newPos.x)
		newPos.x = winViewSize.width - tmapSize.width;
	if (newPos.y > 0) newPos.y = 0;
	if (winViewSize.height - tmapSize.height >= newPos.y)
		newPos.y = winViewSize.height - tmapSize.height;
	pTmap->setPosition(newPos);
	CCLOG("changed pos : %f %f", pTmap->getPosition().x, pTmap->getPosition().y);
}

void MyScene::onTouchEnded(Touch* pTouch, Event* pEvent){
}

void MyScene::onMouseMove(Event* pEvent){
}

void MyScene::onMouseDown(Event* pEvent){
}

void MyScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent){
}

void MyScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent){
}

void MyScene::callEveryFrame(float fDeltaTime){
	ConnectionManager::getInstance()->receive();
	float fElapsedTime = pTimer->getElapsedTime();
	JYObjectManager::getInstance()->tick(fDeltaTime);
}
