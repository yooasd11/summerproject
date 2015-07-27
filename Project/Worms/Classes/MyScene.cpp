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
USING_NS_CC;

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
	CCTMXTiledMap* pTmap = CCTMXTiledMap::create("TileMaps/TestDesert.tmx");
	pTmap->setName("Tmap");
	pBackgroundNode = CCParallaxNode::create();
	pBackgroundNode->setName("Background");
	pBackgroundNode->addChild(pTmap, 1, ccp(1.0f, 1.0f), ccp(0, 0));
	this->addChild(pBackgroundNode, 0);
	CCLOG("TMAP size : (%.1f, %.1f)", pTmap->getContentSize().width, pTmap->getContentSize().height);
	CCTMXObjectGroup* objects = pTmap->objectGroupNamed("Objects");
	CCTMXLayer* metaInfo = pTmap->getLayer("MetaInfo");
	metaInfo->setVisible(false);
}

JYObject* MyScene::createDragon(const UINT& nUID, const CCPoint& dragonPosition, const UINT& nHP){
	CCTexture2D* texture = CCDirector::getInstance()->getTextureCache()->addImage("Animations/dragon_animation.png");
	CCAnimation* animation = Animation::create();
	animation->setDelayPerUnit(0.05f);
	for (int i = 0; i < 6; ++i){
		int index = 1 % 4;
		int rowIndex = i / 4;
		animation->addSpriteFrameWithTexture(texture, Rect(index * 130, rowIndex * 140, 130, 140));
	}

	//Create CCSprite dragon, and add to Tiled map
	CCSprite* pDragon = Sprite::createWithTexture(texture, Rect(0, 0, 130, 140));
	pDragon->setPosition(dragonPosition);
	pDragon->setFlippedX(true);
	pDragon->setScale(0.5);
	pDragon->setName("Dragon");
	CCTMXTiledMap* pTmap = (CCTMXTiledMap*)pBackgroundNode->getChildByName("Tmap");
	pTmap->addChild(pDragon, 3);

	//Run action : flying animation
	Animate* animate = Animate::create(animation);
	RepeatForever* rep = RepeatForever::create(animate);
	pDragon->runAction(rep);

	//Create JYPlayer with CCSprite dragon
	JYPlayer* pJYPlayer = new JYPlayer(pDragon);
	pJYPlayer->setUID(nUID);
	pJYPlayer->setHP(nHP);

	if (nUID == nPlayerUID){
		this->makePlayer(pJYPlayer);
	}
	return pJYPlayer;
}

JYObject* MyScene::createBullet(const UINT& nUID, const UINT& nTh, const cocos2d::CCPoint& pos){
	JYPlayer* pJYPlayer = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pJYPlayer == nullptr) return nullptr;
	CCSprite* pCCOwner = (cocos2d::CCSprite*)pJYPlayer->getCCObject();
	if (pCCOwner == nullptr) return nullptr;

	CCTMXTiledMap* pTmap = GET_TMAP;

	//Create CCSprite bullet
	CCSprite* bullet = CCSprite::create("bullet.PNG");
	bullet->setScale(0.3f);
	bullet->setPosition(pos);

	//Create JYArm with CCSprite bullet and add
	JYArm* pJYArmBullet = new JYArm(bullet);
	pJYPlayer->addChild(pJYArmBullet);
	pJYArmBullet->setObjectType(JYOBJECT_TYPE::JY_ARM);
	pJYArmBullet->setUID(nUID);
	pJYArmBullet->setTag(nTh);
	pTmap->addChild(bullet);
	return pJYArmBullet;
}

//Manager���� ����
//�ڽĵ� ����
//��� ���� �߰� �ҽ� �ʿ���
void MyScene::deleteDragon(const UINT& nUID){
	JYPlayer* pTarget = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pTarget != nullptr){
		//this->getChildByName("Background")->getChildByName("Tmap")->removeChild(pTarget->getCCObject(), true);
		JYObjectManager::getInstance()->removeObject(pTarget);
	}
}

//Make pJYPlayer as controllable player of the scene
void MyScene::makePlayer(JYObject* const pJYPlayer){
	CCNode* pNode = pJYPlayer->getCCObject();

	//make aim
	CCSprite* pFireAim = CCSprite::create("line.PNG");
	pFireAim->setPosition(pNode->getContentSize().width / 2, pNode->getContentSize().height / 3);
	pFireAim->setName("Aim");
	pFireAim->setScale(0.3f);
	pFireAim->setAnchorPoint(ccp(0.0f, -0.5f));
	pNode->addChild(pFireAim);

	pJYPlayerDragon = (JYPlayer*)pJYPlayer;
}


bool MyScene::onTouchBegan(Touch* pTouch, Event* pEvent){
	return true;
}

void MyScene::onTouchMoved(Touch* pTouch, Event* pEvent){
	MyScene* pMyScene = GET_MYSCENE;
	CCTMXTiledMap* pTmap = GET_TMAP;
	CCPoint diff = pTouch->getDelta();
	CCPoint currentPos = pBackgroundNode->getPosition();
	CCPoint newPos = currentPos + diff;
	if (newPos.x < 0) newPos.x = 0;
	if (newPos.x + pTmap->getContentSize().width >= winSize.width) newPos.x = winSize.width - pTmap->getContentSize().width;
	if (newPos.y < 0) newPos.y = 0;
	if (newPos.y + pTmap->getContentSize().height >= winSize.height) newPos.y = winSize.height - pTmap->getContentSize().height;
	pBackgroundNode->setPosition(newPos);
	CCLOG("changed pos : %f %f", pBackgroundNode->getPosition().x, pBackgroundNode->getPosition().y);
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
