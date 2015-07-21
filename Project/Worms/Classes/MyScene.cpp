#include "MyScene.h"
#include "MultiByteConverter.h"
#include "XMLParser.h"
#include "JYObject.h"
#include "JYPlayer.h"
#include "JYItem.h"
#include "JYArm.h"
#include "MoveExecutor.h"
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
	mouseListener->onMouseMove = CC_CALLBACK_1(MyScene::onMouseMove, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(MyScene::onMouseDown, this);

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
	backgroundNode = CCParallaxNode::create();
	backgroundNode->setName("Background");
	backgroundNode->addChild(pTmap, 1, ccp(1.0f, 1.0f), ccp(0, 0));
	this->addChild(backgroundNode, 0);
	CCLOG("TMAP size : (%.1f, %.1f)", pTmap->getContentSize().width, pTmap->getContentSize().height);
	CCTMXObjectGroup* objects = pTmap->objectGroupNamed("Objects");
	CCTMXLayer* metaInfo = pTmap->getLayer("MetaInfo");
	metaInfo->setVisible(false);
}

void MyScene::createDragon(const UINT& nUID, const CCPoint& dragonPosition){
	CCTexture2D* texture = CCDirector::getInstance()->getTextureCache()->addImage("Animations/dragon_animation.png");
	CCAnimation* animation = Animation::create();
	animation->setDelayPerUnit(0.05f);
	for (int i = 0; i < 6; ++i){
		int index = 1 % 4;
		int rowIndex = i / 4;
		animation->addSpriteFrameWithTexture(texture, Rect(index * 130, rowIndex * 140, 130, 140));
	}

	CCSprite* pDragon = Sprite::createWithTexture(texture, Rect(0, 0, 130, 140));
	pDragon->setPosition(dragonPosition);
	pDragon->setFlippedX(true);
	pDragon->setScale(0.5);
	pDragon->setName("Dragon");
	CCTMXTiledMap* pTmap = (CCTMXTiledMap*)backgroundNode->getChildByName("Tmap");
	pTmap->addChild(pDragon, 3, 15);

	Animate* animate = Animate::create(animation);
	RepeatForever* rep = RepeatForever::create(animate);
	pDragon->runAction(rep);

	JYPlayer* pJYPlayer = new JYPlayer(pDragon);
	pJYPlayer->setUID(nUID);
	if (nUID == nPlayerUID){
		this->makePlayer(pJYPlayer);
	}
}

void MyScene::deleteDragon(const UINT& nUID){
	JYPlayer* pTarget = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	this->removeChild(pTarget->getCCObject());
	delete pTarget;
}

void MyScene::makePlayer(JYObject* const pJYPlayer){
	CCNode* pNode = pJYPlayer->getCCObject();

	pFireAim = CCSprite::create("line.PNG");
	pFireAim->setPosition(pNode->getContentSize().width / 2, pNode->getContentSize().height / 3);
	pFireAim->setName("Aim");
	pFireAim->setScale(0.3f);
	pFireAim->setAnchorPoint(ccp(-0.5f, 0.5f));
	pNode->addChild(pFireAim);

	CCSprite* bullet = CCSprite::create("bullet.PNG");
	bullet->setName("Bullet");
	bullet->setVisible(false);
	bullet->setScale(0.3f);
	bullet->setPosition(ccp(5.0f, 5.0f));
	addChild(bullet);

	pJYPlayerDragon = (JYPlayer*)pJYPlayer;
	pJYArmBullet = new JYArm(bullet);
	pJYArmBullet->setName("JYBullet");
	pJYPlayerDragon->addChild(pJYArmBullet);
}


bool MyScene::onTouchBegan(Touch* pTouch, Event* pEvent){
	return true;
}

void MyScene::onTouchMoved(Touch* pTouch, Event* pEvent){
	CCTMXTiledMap* pTmap = (CCTMXTiledMap*)pJYPlayerDragon->getCCObject()->getParent();
	CCPoint diff = pTouch->getDelta();
	CCPoint currentPos = backgroundNode->getPosition();
	CCPoint newPos = currentPos + diff;
	if (newPos.x < 0) newPos.x = 0;
	if (newPos.x + pTmap->getContentSize().width >= winSize.width) newPos.x = winSize.width - pTmap->getContentSize().width;
	if (newPos.y < 0) newPos.y = 0;
	if (newPos.y + pTmap->getContentSize().height >= winSize.height) newPos.y = winSize.height - pTmap->getContentSize().height;
	backgroundNode->setPosition(newPos);
	CCLOG("changed pos : %f %f", backgroundNode->getPosition().x, backgroundNode->getPosition().y);
}

void MyScene::onTouchEnded(Touch* pTouch, Event* pEvent){
}

void MyScene::onMouseMove(Event* pEvent){
	if (pJYPlayerDragon != nullptr)
		pJYPlayerDragon->onMouseMove(pEvent);
}

void MyScene::onMouseDown(Event* pEvent){
	if (pJYPlayerDragon != nullptr)
		pJYPlayerDragon->onMouseDown(pEvent);
}

void MyScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent){
}

void MyScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent){
}

void MyScene::callEveryFrame(float fDeltaTime){
	ConnectionManager::getInstance()->receive();
	float fElapsedTime = pTimer->getElapsedTime();
	if (pJYPlayerDragon != nullptr){

		std::vector<JYObject*>* pJYObjectArray = &JYObjectManager::getInstance()->m_pJYObjectVector;
		for (auto it = pJYObjectArray->begin(); it < pJYObjectArray->end(); ++it){
			(*it)->tick(fElapsedTime);
		}
		pJYArmBullet->tick(fElapsedTime);
	}
}
