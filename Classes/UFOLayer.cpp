#include "UFOLayer.h"

UFOLayer::UFOLayer(void)
{
	m_pAllMutiBullets = __Array::create();
	m_pAllMutiBullets->retain();
	m_pAllBigBoom = __Array::create();
	m_pAllBigBoom->retain();
}

UFOLayer::~UFOLayer(void)
{
	m_pAllMutiBullets->release();
	m_pAllMutiBullets = nullptr;
	m_pAllBigBoom->release();
	m_pAllBigBoom = nullptr;
}

bool UFOLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		this->schedule(schedule_selector(UFOLayer::AddMutiBullets),20.0);
		this->schedule(schedule_selector(UFOLayer::AddBigBoom),20.0,kRepeatForever,5.0);
		
        bRet = true;
	} while (0);

	return bRet;
}

void UFOLayer::AddMutiBullets(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("out_porp.mp3");
	auto mutiBullets = Sprite::createWithSpriteFrameName("ufo1.png");

	auto mutiBlletsSize = mutiBullets->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = mutiBlletsSize.width/2;
	int maxX = winSize.width-mutiBlletsSize.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	mutiBullets->setPosition(Point(actualX, winSize.height+mutiBlletsSize.height/2));
	this->addChild(mutiBullets);
	this->m_pAllMutiBullets->addObject(mutiBullets);

	auto move1 = MoveBy::create(0.5f, Point(0, -150));
	auto move2 = MoveBy::create(0.3f, Point(0, 100));
	auto move3 = MoveBy::create(1.0f, Point(0, 0-winSize.height-mutiBlletsSize.height/2));
	auto moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::mutiBulletsMoveFinished, this));

	auto sequence = Sequence::create(move1, move2, move3, moveDone, nullptr);
	mutiBullets->runAction(sequence);
}

void UFOLayer::mutiBulletsMoveFinished(Node* pSender)
{
	auto mutiBullets = (Sprite*)pSender;
	this->removeChild(mutiBullets, true);
	this->m_pAllMutiBullets->removeObject(mutiBullets);
}

void UFOLayer::AddBigBoom(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("out_porp.mp3");
	auto bigBoom = Sprite::createWithSpriteFrameName("ufo2.png");

	auto bigBoomSize = bigBoom->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = bigBoomSize.width/2;
	int maxX = winSize.width-bigBoomSize.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	bigBoom->setPosition(Point(actualX, winSize.height+bigBoomSize.height/2));
	this->addChild(bigBoom);
	this->m_pAllBigBoom->addObject(bigBoom);

	auto move1 = MoveBy::create(0.5, Point(0, -150));
	auto move2 = MoveBy::create(0.3, Point(0, 100));
	auto move3 = MoveBy::create(1.0, Point(0, 0-winSize.height-bigBoomSize.height/2));
	auto moveDone = CCCallFuncN::create(CC_CALLBACK_1(UFOLayer::bigBoomMoveFinished, this));

	auto sequence = Sequence::create(move1, move2, move3, moveDone, nullptr);
	bigBoom->runAction(sequence);
}

void UFOLayer::bigBoomMoveFinished(Node* pSender)
{
	auto bigBoom = (Sprite*)pSender;
	this->removeChild(bigBoom, true);
	this->m_pAllBigBoom->removeObject(bigBoom);
}

void UFOLayer::RemoveMutiBullets(Sprite* mutiBullets)
{
	this->removeChild(mutiBullets, true);
	this->m_pAllMutiBullets->removeObject(mutiBullets);
}

void UFOLayer::RemoveBigBoom(Sprite* bigBoom)
{
	this->removeChild(bigBoom, true);
	this->m_pAllBigBoom->removeObject(bigBoom);
}