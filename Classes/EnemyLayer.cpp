#include "EnemyLayer.h"
#include "GameLayer.h"

EnemyLayer::EnemyLayer(void)
{
	enemy1SpriteFrame = nullptr;
	enemy2SpriteFrame = nullptr;
	enemy3SpriteFrame_1 = nullptr;
	enemy3SpriteFrame_2 = nullptr;

	m_pAllEnemy1 = __Array::create();
	m_pAllEnemy1->retain();
	m_pAllEnemy2 = __Array::create();
	m_pAllEnemy2->retain();
	m_pAllEnemy3 = __Array::create();
	m_pAllEnemy3->retain();
}

EnemyLayer::~EnemyLayer(void)
{
	m_pAllEnemy1->release();
	m_pAllEnemy1 = nullptr;
	m_pAllEnemy2->release();
	m_pAllEnemy2 = nullptr;
	m_pAllEnemy3->release();
	m_pAllEnemy3 = nullptr;
}

bool EnemyLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		enemy1SpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1.png");
		enemy2SpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png");
		enemy3SpriteFrame_1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png");
		enemy3SpriteFrame_2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png");

		auto animation1 = Animation::create();
		animation1->setDelayPerUnit(0.1f);
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down1.png"));
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down2.png"));
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down3.png"));
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down4.png"));

		auto animation2 = Animation::create();
		animation2->setDelayPerUnit(0.1f);
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down1.png"));
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down2.png"));
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down3.png"));
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down4.png"));

		auto animation3 = Animation::create();
		animation3->setDelayPerUnit(0.1f);
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down1.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down2.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down3.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down4.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down5.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down6.png"));

		CCAnimationCache::getInstance()->addAnimation(animation1, "Enemy1Blowup");
		CCAnimationCache::getInstance()->addAnimation(animation2, "Enemy2Blowup");
		CCAnimationCache::getInstance()->addAnimation(animation3, "Enemy3Blowup");

		this->schedule(schedule_selector(EnemyLayer::addEnemy1),0.5f);
		this->schedule(schedule_selector(EnemyLayer::addEnemy2),3.0f);
		this->schedule(schedule_selector(EnemyLayer::addEnemy3),6.0f);

		bRet = true;
	} while (0);
	return bRet;
}


void EnemyLayer::addEnemy1(float dt)
{
	auto enemy1 = Enemy::create();
	enemy1->bindSprite(Sprite::createWithSpriteFrame(enemy1SpriteFrame), ENEMY1_MAXLIFE);

	auto enemy1Size = enemy1->getSprite()->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = enemy1Size.width/2;
	int maxX = winSize.width-enemy1Size.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	enemy1->setPosition(Point(actualX, winSize.height+enemy1Size.height/2));
	this->addChild(enemy1);
	this->m_pAllEnemy1->addObject(enemy1);

	float minDuration, maxDuration;
	switch(GameLayer::getCurLevel())
	{
	case EASY:
		minDuration = 2.0f;
		maxDuration = 4.0f;
		break;
	case MIDDLE:
		minDuration = 1.8f;
		maxDuration = 3.6f;
		break;
	case HARD:
		minDuration = 1.6f;
		maxDuration = 3.2f;
		break;
	default:
		minDuration = 2.0f;
		maxDuration = 4.0f;
		break;
	}

	int rangeDuration = maxDuration-minDuration;
	int actualDuration = (rand()%rangeDuration)+minDuration;

	auto actionMove = MoveTo::create(actualDuration, Point(actualX, 0-enemy1->getSprite()->getContentSize().height/2));
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(EnemyLayer::enemy1MoveFinished, this));

	auto sequence = Sequence::create(actionMove, actionDone, nullptr);
	enemy1->runAction(sequence);

}

void EnemyLayer::enemy1MoveFinished(Node* pSender)
{
	auto enmey1 = (Enemy*)pSender;
	this->removeChild(enmey1, true);
	this->m_pAllEnemy1->removeObject(enmey1);
}

void EnemyLayer::enemy1Blowup(Enemy* enemy1)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/enemy1_down.mp3");
	auto animation = AnimationCache::getInstance()->getAnimation("Enemy1Blowup");
	auto animate = Animate::create(animation);
	auto removeEnemy1 = CallFunc::create(CC_CALLBACK_0(EnemyLayer::removeEnemy1, this, enemy1));
	auto sequence = Sequence::create(animate, removeEnemy1, nullptr);
	enemy1->getSprite()->runAction(sequence);
}

void EnemyLayer::removeEnemy1(Node* pTarget)
{
	auto enemy1 = (Enemy*)pTarget;
	if (enemy1 != nullptr)
	{
		m_pAllEnemy1->removeObject(enemy1);
		this->removeChild(enemy1, true);
	}
}

void EnemyLayer::removeAllEnemy1()
{
	Ref* obj;
	CCARRAY_FOREACH(m_pAllEnemy1, obj)
	{
		auto enemy1 = (Enemy*)obj;
		if (enemy1->getLife() > 0)
		{
			enemy1Blowup(enemy1);
		}
	}
}


void EnemyLayer::addEnemy2(float dt)
{
	auto enemy2 = Enemy::create();
	enemy2->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE);

	auto enemy2Size = enemy2->getSprite()->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = enemy2Size.width/2;
	int maxX = winSize.width-enemy2Size.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	enemy2->setPosition(Point(actualX,winSize.height+enemy2Size.height/2));
	this->addChild(enemy2);
	this->m_pAllEnemy2->addObject(enemy2);

	float minDuration, maxDuration;
	switch(GameLayer::getCurLevel())
	{
	case EASY:
		minDuration = 3.0;
		maxDuration = 6.0;
		break;
	case MIDDLE:
		minDuration = 2.7;
		maxDuration = 5.4;
		break;
	case HARD:
		minDuration = 2.5;
		maxDuration = 5.0;
		break;
	default:
		minDuration = 3.0;
		maxDuration = 6.0;
		break;
	}
	int rangeDuration = maxDuration-minDuration;
	int actualDuration = (rand()%rangeDuration)+minDuration;

	auto actionMove = MoveTo::create(actualDuration,Point(actualX, 0-enemy2->getSprite()->getContentSize().height/2));
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(EnemyLayer::enemy2MoveFinished, this));

	auto sequence = Sequence::create(actionMove, actionDone, nullptr);
	enemy2->runAction(sequence);
}

void EnemyLayer::enemy2MoveFinished(Node* pSender)
{
	auto enmey2 = (Enemy*)pSender;
	this->removeChild(enmey2, true);
	this->m_pAllEnemy2->removeObject(enmey2);
}

void EnemyLayer::enemy2Blowup(Enemy* enemy2)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/enemy2_down.mp3");
	auto animation = AnimationCache::getInstance()->getAnimation("Enemy2Blowup");
	auto animate = Animate::create(animation);

	auto removeEnemy2 = CallFunc::create(CC_CALLBACK_0(EnemyLayer::removeEnemy2, this, enemy2));
	auto sequence = Sequence::create(animate, removeEnemy2, NULL);
	enemy2->getSprite()->runAction(sequence);
}

void EnemyLayer::removeEnemy2(Node* pTarget)
{
	auto enemy2 = (Enemy*)pTarget;
	if (enemy2 != nullptr)
	{
		m_pAllEnemy2->removeObject(enemy2);
		this->removeChild(enemy2, true);
	}
}

void EnemyLayer::removeAllEnemy2()
{
	Ref* obj;
	CCARRAY_FOREACH(m_pAllEnemy2,obj)
	{
		auto enemy2 = (Enemy*)obj;
		if (enemy2->getLife()>0)
		{
			enemy2Blowup(enemy2);
		}
	}
}


void EnemyLayer::addEnemy3(float dt)
{
	auto enemy3 = Enemy::create();
	enemy3->bindSprite(Sprite::createWithSpriteFrame(enemy3SpriteFrame_1), ENEMY3_MAXLIFE);

	auto enemy3Size = enemy3->getSprite()->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = enemy3Size.width/2;
	int maxX = winSize.width-enemy3Size.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	enemy3->setPosition(Point(actualX, winSize.height+enemy3Size.height/2));
	this->addChild(enemy3);
	this->m_pAllEnemy3->addObject(enemy3);

	float minDuration, maxDuration;
	switch(GameLayer::getCurLevel())
	{
	case EASY:
		minDuration = 4.0;
		maxDuration = 8.0;
		break;
	case MIDDLE:
		minDuration = 3.6;
		maxDuration = 7.2;
		break;
	case HARD:
		minDuration = 3.2;
		maxDuration = 6.4;
		break;
	default:
		minDuration = 4.0;
		maxDuration = 8.0;
		break;
	}

	int rangeDuration = maxDuration-minDuration;
	int actualDuration = (rand()%rangeDuration)+minDuration;

	auto actionMove = MoveTo::create(actualDuration, Point(actualX, 0-enemy3->getSprite()->getContentSize().height/2));
	auto actionDone = CallFunc::create(CC_CALLBACK_0(EnemyLayer::enemy3MoveFinished, this, enemy3));

	auto sequence = Sequence::create(actionMove, actionDone, nullptr);
	enemy3->runAction(sequence);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2f);
	animation->addSpriteFrame(enemy3SpriteFrame_1);
	animation->addSpriteFrame(enemy3SpriteFrame_2);
	auto animate = Animate::create(animation);
	enemy3->getSprite()->runAction(RepeatForever::create(animate));
}

void EnemyLayer::enemy3MoveFinished(Node* pSender)
{
	auto enmey3 = (Enemy*)pSender;
	this->removeChild(enmey3, true);
	this->m_pAllEnemy3->removeObject(enmey3);
}

void EnemyLayer::enemy3Blowup(Enemy* enemy3)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/enemy3_down.mp3");
    auto animation = AnimationCache::getInstance()->getAnimation("Enemy3Blowup");

	auto animate = Animate::create(animation);
	auto removeEnemy3 = CallFunc::create(CC_CALLBACK_0(EnemyLayer::removeEnemy3, this, enemy3));
	auto sequence = Sequence::create(animate, removeEnemy3, nullptr);
	enemy3->getSprite()->runAction(sequence);
}

void EnemyLayer::removeEnemy3(Node* pTarget)
{
	auto enemy3 = (Enemy*)pTarget;
	if (enemy3 != nullptr)
	{
		enemy3->stopAllActions();
		m_pAllEnemy3->removeObject(enemy3);
		this->removeChild(enemy3, true);
	}
}

void EnemyLayer::removeAllEnemy3()
{
	Ref* obj;
	CCARRAY_FOREACH(m_pAllEnemy3, obj)
	{
		auto enemy3 = (Enemy*)obj;
		if (enemy3->getLife()>0)
		{
			enemy3Blowup(enemy3);
		}
	}
}

void EnemyLayer::removeAllEnemy()
{
	removeAllEnemy1();
	removeAllEnemy2();
	removeAllEnemy3();
}
