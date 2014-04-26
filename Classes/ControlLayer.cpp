#include "ControlLayer.h"

ControlLayer::ControlLayer(void)
{
	scoreItem = nullptr;
	pPauseItem = nullptr;
}

ControlLayer::~ControlLayer(void)
{
}

bool ControlLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		auto winSize = Director::getInstance()->getWinSize();

		auto normalPause = Sprite::createWithSpriteFrameName("game_pause_nor.png");
		auto pressedPause = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
		pPauseItem = MenuItemSprite::create(normalPause, pressedPause, CC_CALLBACK_1(ControlLayer::menuPauseCallback, this));
		pPauseItem->setPosition(Point(normalPause->getContentSize().width/2+10, winSize.height-normalPause->getContentSize().height/2-10));
        auto menuPause = Menu::create(pPauseItem, nullptr);
		menuPause->setPosition(Point::ZERO);
		this->addChild(menuPause,101);

		scoreItem = Label::createWithBMFont("font.fnt", "0");
		scoreItem->setColor(Color3B(143,146,147));
		scoreItem->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
		scoreItem->setPosition(Point(pPauseItem->getPositionX()+normalPause->getContentSize().width/2+5, pPauseItem->getPositionY()));
		this->addChild(scoreItem);

		bRet = true;
	} while (0);

	return bRet;
}

void ControlLayer::menuPauseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button.mp3");
	if(!CCDirector::getInstance()->isPaused())
	{
		pPauseItem->setNormalImage(Sprite::createWithSpriteFrameName("game_resume_nor.png"));
		pPauseItem->setSelectedImage(Sprite::createWithSpriteFrameName("game_resume_pressed.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		Director::getInstance()->pause();
		noTouchLayer = NoTouchLayer::create();
		this->addChild(noTouchLayer);
	}
	else
	{
		pPauseItem->setNormalImage(Sprite::createWithSpriteFrameName("game_pause_nor.png"));
		pPauseItem->setSelectedImage(Sprite::createWithSpriteFrameName("game_pause_pressed.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		Director::getInstance()->resume();
		this->removeChild(noTouchLayer,true);
	}
}

void ControlLayer::updateScore(int score)
{
	if (score>=0 && score<=MAX_SCORE)
	{
        Value strScore(score);
		scoreItem->setString(strScore.asString());
	}
}
