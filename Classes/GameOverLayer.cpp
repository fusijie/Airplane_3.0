#include "GameOverLayer.h"
#include "GameScene.h"

int GameOverLayer::highestHistoryScore = 0;

GameOverLayer::GameOverLayer(void)
{
	score = 0;
	highestScore = nullptr;
}

GameOverLayer::~GameOverLayer(void)
{
}

GameOverLayer* GameOverLayer::create(int passScore)
{
	GameOverLayer *pRet = new GameOverLayer();
	pRet->score = passScore;
	if (pRet && pRet->init())
	{ 
		pRet->autorelease();
	} 
	else 
	{ 
		delete pRet; 
		pRet = nullptr;
	}
    return pRet;
}

bool GameOverLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		}
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("game_over.mp3");

		auto winSize = Director::getInstance()->getWinSize();

	    Sprite* background = Sprite::createWithSpriteFrameName("gameover.png");
		background->setPosition(Point(winSize.width/2, winSize.height/2));
		this->addChild(background);

		auto normalBackToGame = Sprite::createWithSpriteFrameName("btn_finish.png");
		auto pressedBackToGame = Sprite::createWithSpriteFrameName("btn_finish.png");
		auto pBackItem = MenuItemSprite::create(normalBackToGame,
                                                pressedBackToGame,
                                                nullptr,
                                                CC_CALLBACK_1(GameOverLayer::menuBackCallback, this));
		pBackItem->setPosition(Point(winSize.width-normalBackToGame->getContentSize().width/2-10, normalBackToGame->getContentSize().height/2+10));
		auto menuBack = Menu::create(pBackItem, nullptr);
		menuBack->setPosition(Point::ZERO);
		this->addChild(menuBack);

		auto strScore = CCString::createWithFormat("%d",score);
		auto finalScore = LabelBMFont::create(strScore->getCString(), "font.fnt");
		finalScore->setColor(Color3B(143,146,147));
		finalScore->setPosition(Point(winSize.width/2, winSize.height/2));
		this->addChild(finalScore);

		auto delay = DelayTime::create(1.0f);
		auto scalebig = ScaleTo::create(1.0f,3.0f);
		auto scalelittle = ScaleTo::create(0.3f,2.0f);
		auto showAD = CallFunc::create(CC_CALLBACK_0(GameOverLayer::showAD, this));
		auto sequence = Sequence::create(delay, scalebig, scalelittle, showAD, nullptr);
		finalScore->runAction(sequence);

		auto strHighestScore = CCString::createWithFormat("%d", highestHistoryScore);
		highestScore = LabelBMFont::create(strHighestScore->getCString(), "font.fnt");
		highestScore->setColor(Color3B(143,146,147));
		highestScore->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
		highestScore->setPosition(Point(140,winSize.height-30));
		this->addChild(highestScore);

		if (score>highestHistoryScore)
		{
			UserDefault::getInstance()->setIntegerForKey("HighestScore", score);
			highestHistoryScore = score;
			auto delayChange = DelayTime::create(1.3f);
			auto moveOut = MoveBy::create(0.1f, Point(0,100));
			auto beginChange = CallFuncN::create(CC_CALLBACK_1(GameOverLayer::beginChangeHighestScore, this));
			auto moveIn = MoveBy::create(0.1f,Point(0,-100));
			auto sequence = Sequence::create(delayChange, moveOut, beginChange, moveIn, nullptr);
			highestScore->runAction(sequence);
		}

		bRet = true;
	} while (0);
	return bRet;
}

void GameOverLayer::menuBackCallback(Ref* pSender)
{
	auto pScene = GameScene::create();
	auto animateScene = TransitionSlideInL::create(1.0f, pScene);
	CCDirector::getInstance()->replaceScene(animateScene);
}

void GameOverLayer::beginChangeHighestScore(Node* pNode)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("achievement.mp3");
	auto changeScore = CCString::createWithFormat("%d",score);
	highestScore->setString(changeScore->getCString());
}

void GameOverLayer::showAD()
{
	
}
