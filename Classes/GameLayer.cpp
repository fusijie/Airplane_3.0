#include "GameLayer.h"
#include "Enemy.h"


Level GameLayer::level = EASY;

GameLayer::GameLayer(void)
{
	background1 = nullptr;
	background2 = nullptr;
	planeLayer = nullptr;
	bulletLayer = nullptr;
	mutiBulletsLayer = nullptr;
	controlLayer = nullptr;
	enemyLayer = nullptr;

	score = 0;
	bigBoomCount = 0;
}

GameLayer::~GameLayer(void)
{
}

bool GameLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		level = EASY;

		if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("game_music.mp3",true);
		}

		background1 = Sprite::createWithSpriteFrameName("background.png");
		background1->setAnchorPoint(Point::ZERO);
		background1->setPosition(Point::ZERO);
        background1->getTexture()->setAliasTexParameters();
		this->addChild(background1);

		background2 = Sprite::createWithSpriteFrameName("background.png");
		background2->setAnchorPoint(Point::ZERO);
		background2->setPosition(Point(0,background2->getContentSize().height-2));
        background2->getTexture()->setAliasTexParameters();
		this->addChild(background2);

		this->planeLayer = PlaneLayer::create();
		this->addChild(planeLayer);

		this->bulletLayer = BulletLayer::create();
		this->addChild(bulletLayer);
		this->bulletLayer->StartShoot();

		this->mutiBulletsLayer = MutiBulletsLayer::create();
		this->addChild(mutiBulletsLayer);

		this->enemyLayer = EnemyLayer::create();
		this->addChild(enemyLayer);

		this->controlLayer = ControlLayer::create();
		this->addChild(controlLayer);

		this->ufoLayer = UFOLayer::create();
		this->addChild(ufoLayer);

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
		this->schedule(schedule_selector(GameLayer::backgroundMove),0.01f);

		this->scheduleUpdate();

		bRet = true;
	} while (0);
	return bRet;
}

void GameLayer::backgroundMove(float dt)
{
	background1->setPositionY(int(background1->getPositionY())-2);
	background2->setPositionY(int(background1->getPositionY())+int(background1->getContentSize().height)-2);
	if (background2->getPositionY() == 0)
	{
		background1->setPositionY(0);
	}
}

bool GameLayer::onTouchBegan(Touch* touch, Event  *event)
{
	return true;
}

void GameLayer::onTouchMoved(Touch* touch, Event  *event)
{
	if (this->planeLayer->isAlive)
	{
		Point beginPoint = touch->getLocation();
		//juggle the area of drag 
		Rect planeRect = this->planeLayer->getChildByTag(AIRPLANE)->boundingBox();
		planeRect.origin.x -= 15;
		planeRect.origin.y -= 15;
		planeRect.size.width += 30;
		planeRect.size.height += 30;
		if(planeRect.containsPoint(this->getParent()->convertTouchToNodeSpace(touch)))
		{ 
			Point endPoint = touch->getPreviousLocation();

			Point offSet = beginPoint - endPoint;
			Point toPoint = this->planeLayer->getChildByTag(AIRPLANE)->getPosition() + offSet;
			this->planeLayer->MoveTo(toPoint); 
		} 
	}
}

void GameLayer::onTouchEnded(Touch* touch, Event  *event)
{
	
}

void GameLayer::update(float dt)
{
	if (level == EASY && score >= 1000000)
	{
		level = MIDDLE;
	}
	else if (level == MIDDLE && score >= 2000000)
	{
		level = HARD;
	}

	auto bulletsToDelete = __Array::create();
	bulletsToDelete->retain();
	Ref* bt,*et,*ut;

	//enemy1 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet, bt)
	{
		auto bullet = (Sprite*)bt;
		
		auto enemy1sToDelete = CCArray::create();
		enemy1sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1, et)
		{
			auto enemy1 = (Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife() == 1)
				{
					enemy1->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy1sToDelete->addObject(enemy1);
					score += ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete, et)
		{
			auto enemy1 = (Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete, bt)
	{
		auto bullet = (Sprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy2 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet, bt)
	{
		auto bullet = (Sprite*)bt;

		auto enemy2sToDelete = __Array::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2, et)
		{
			auto enemy2 = (Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife() > 1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if(enemy2->getLife() == 1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy2sToDelete->addObject(enemy2);
					score += ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete, et)
		{
			auto enemy2 = (Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete, bt)
	{
		auto bullet = (Sprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy3 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet, bt)
	{
		auto bullet = (Sprite*)bt;

		auto enemy3sToDelete=__Array::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3, et)
		{
			auto enemy3 = (Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if(enemy3->getLife() == 1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy3sToDelete->addObject(enemy3);
					score += ENEMY3_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete, et)
		{
			auto enemy3 = (Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete, bt)
	{
		auto bullet = (Sprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();
	bulletsToDelete->release();


	auto mutiBulletsToDelete = __Array::create();
	mutiBulletsToDelete->retain();
	Ref* mbt;

	////enemy1 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;

		auto enemy1sToDelete = __Array::create();
		enemy1sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et)
		{
			auto enemy1 = (Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife() == 1)
				{
					enemy1->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy1sToDelete->addObject(enemy1);
					score += ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete, et)
		{
			auto enemy1 = (Enemy*) et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	//enemy2 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;

		auto enemy2sToDelete = __Array::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2, et)
		{
			auto enemy2 = (Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife() > 1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				else if(enemy2->getLife() == 1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy2sToDelete->addObject(enemy2);
					score += ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete, et)
		{
			auto enemy2 = (Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	//enemy3 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;

		auto enemy3sToDelete = __Array::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3, et)
		{
			auto enemy3 = (Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				if (enemy3->getLife() > 1)
				{
					enemy3->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				else if(enemy3->getLife() == 1)
				{
					enemy3->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy3sToDelete->addObject(enemy3);
					score += ENEMY3_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete, et)
		{
			auto enemy3 = (Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();
	mutiBulletsToDelete->release();


	auto airplaneRect = this->planeLayer->getChildByTag(AIRPLANE)->boundingBox();
	airplaneRect.origin.x += 30;
	airplaneRect.size.width -= 60;
	//enemy1 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1, et)
	{
		auto enemy1 = (Enemy*)et;
		if (enemy1->getLife() > 0)
		{
			if (airplaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	//enemy2 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2, et)
	{
		auto enemy2 = (Enemy*)et;
		if (enemy2->getLife() > 0)
		{
			if (airplaneRect.intersectsRect(enemy2->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	//enemy3 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3, et)
	{
		auto enemy3 = (Enemy*)et;
		if (enemy3->getLife() > 0)
		{
			if (airplaneRect.intersectsRect(enemy3->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	//mutiBullets & airplane CheckCollision
	CCARRAY_FOREACH(this->ufoLayer->m_pAllMutiBullets, ut)
	{
		auto mutiBullets = (Sprite*)ut;
		if (this->planeLayer->getChildByTag(AIRPLANE)->boundingBox().intersectsRect(mutiBullets->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get_double_laser.mp3");
			this->ufoLayer->RemoveMutiBullets(mutiBullets);
			this->bulletLayer->StopShoot();
			this->mutiBulletsLayer->StartShoot();
			this->bulletLayer->StartShoot(6.2f);
		}
	}

	//bigBoom & airplane CheckCollision
	CCARRAY_FOREACH(this->ufoLayer->m_pAllBigBoom, ut)
	{
		auto bigBoom = (Sprite*)ut;
		if (this->planeLayer->getChildByTag(AIRPLANE)->boundingBox().intersectsRect(bigBoom->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get_bomb.mp3");
			this->ufoLayer->RemoveBigBoom(bigBoom);
			bigBoomCount++;
			updateBigBoomItem(bigBoomCount);
		}
	}
}

void GameLayer::updateBigBoomItem(int bigBoomCount)
{
	auto normalBomb = Sprite::createWithSpriteFrameName("bomb.png");
	auto pressedBomb = Sprite::createWithSpriteFrameName("bomb.png");
	if (bigBoomCount < 0)
	{
		return;
	}
	else if (bigBoomCount == 0)
	{
		if(this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			this->removeChildByTag(TAG_BIGBOOM_MENUITEM, true);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL, true);
		}
	}
	else if (bigBoomCount == 1)
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			auto pBigBoomItem = MenuItemSprite::create(normalBomb, pressedBomb, nullptr, CC_CALLBACK_1(GameLayer::menuBigBoomCallback, this));
			pBigBoomItem->setPosition(Point(normalBomb->getContentSize().width/2+10, normalBomb->getContentSize().height/2+10));
			menuBigBoom = Menu::create(pBigBoomItem, nullptr);
			menuBigBoom->setPosition(Point::ZERO);
			this->addChild(menuBigBoom, 0, TAG_BIGBOOM_MENUITEM);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
	}
	else
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			auto pBigBoomItem = MenuItemSprite::create(normalBomb, pressedBomb, nullptr, CC_CALLBACK_1(GameLayer::menuBigBoomCallback, this));
			pBigBoomItem->setPosition(Point(normalBomb->getContentSize().width/2+10, normalBomb->getContentSize().height/2+10));
			menuBigBoom = Menu::create(pBigBoomItem, nullptr);
			menuBigBoom->setPosition(Point::ZERO);
			this->addChild(menuBigBoom, 0, TAG_BIGBOOM_MENUITEM);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
		if (bigBoomCount >= 0 && bigBoomCount <= MAX_BIGBOOM_COUNT)
		{
			auto strScore = __String::createWithFormat("X%d", bigBoomCount);
            bigBoomCountItem = Label::createWithBMFont("font.fnt", strScore->getCString());
			bigBoomCountItem->setColor(Color3B(143, 146, 147));
			bigBoomCountItem->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
			bigBoomCountItem->setPosition(Point(normalBomb->getContentSize().width+15, normalBomb->getContentSize().height/2+5));
			this->addChild(bigBoomCountItem, 0, TAG_BIGBOOMCOUNT_LABEL);
		}
	}
}

void GameLayer::menuBigBoomCallback(Ref* pSender)
{
	if(bigBoomCount > 0 && !Director::getInstance()->isPaused())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("use_bomb.mp3");
		bigBoomCount--;
		score += this->enemyLayer->m_pAllEnemy1->count() * ENEMY1_SCORE;
		score += this->enemyLayer->m_pAllEnemy2->count() * ENEMY2_SCORE;
		score += this->enemyLayer->m_pAllEnemy3->count() * ENEMY3_SCORE;
		this->enemyLayer->removeAllEnemy();
		updateBigBoomItem(bigBoomCount);
		this->controlLayer->updateScore(score);
	}
}

Level GameLayer::getCurLevel()
{
	return level;
}