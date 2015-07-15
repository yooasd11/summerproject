#include "MyScene.h"
#include "MultiByteConverter.h"
#include "XMLParser.h"
#include "JYObject.h"
#include "MoveExecutor.h"
#include "Inputhandler.h"
USING_NS_CC;

Scene* MyScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MyScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
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

	pJYObjectDragon = nullptr;
	winSize = CCDirector::getInstance()->getWinSize();
	CCLOG("WinSize : (%.2f, %.2f)", winSize.width, winSize.height);

	this->addBackground();

	this->schedule(schedule_selector(MyScene::callEveryFrame));
	return true;

}

void MyScene::createDragon(CCPoint dragonPosition){
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
	

	CCSprite* fireAim = CCSprite::create("line.PNG");
	fireAim->setPosition(pDragon->getContentSize().width / 2, pDragon->getContentSize().height / 3);
	fireAim->setName("Aim");
	fireAim->setScale(0.3f);
	fireAim->setAnchorPoint(ccp(-0.5f, 0.5f));
	pDragon->addChild(fireAim, 2);

	Animate* animate = Animate::create(animation);
	RepeatForever* rep = RepeatForever::create(animate);
	pDragon->runAction(rep);

	pJYObjectDragon = new JYObject(pDragon);
}

void MyScene::addBackground(){
	CCTMXTiledMap* pTmap = CCTMXTiledMap::create("TileMaps/TestDesert.tmx");
	pTmap->setName("Tmap");
	backgroundNode = CCParallaxNode::create();
	backgroundNode->addChild(pTmap, 1, ccp(1.0f, 1.0f), ccp(0, 0));

	this->addChild(backgroundNode, 0);

	CCTMXObjectGroup* objects = pTmap->objectGroupNamed("Objects");
	ValueMap& spawnPoint = objects->getObject("SpawnPoint");
	CCTMXLayer* metaInfo = pTmap->getLayer("MetaInfo");
	metaInfo->setVisible(false);

	//create a dragon on "SpawnPoint"
	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	this->createDragon(CCPoint(ccp(x, y)));
}

void MyScene::onEnter(){
	CCLayer::onEnter();

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


bool MyScene::onTouchBegan(Touch* pTouch, Event* pEvent){
	return true;
}

void MyScene::onTouchMoved(Touch* pTouch, Event* pEvent){
	CCTMXTiledMap* pTmap = (CCTMXTiledMap*)pJYObjectDragon->getCCObject()->getParent();
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
	InputHandler::getInstance()->onMouseMove(pEvent);
}

void MyScene::onMouseDown(Event* pEvent){
}

void MyScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent){
	InputHandler::getInstance()->onKeyPressed(keyCode, pEvent);
}

void MyScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent){
	InputHandler::getInstance()->onKeyPressed(keyCode, pEvent);
}

void MyScene::callEveryFrame(float fDeltaTime){
	pJYObjectDragon->tick(fDeltaTime);
}
