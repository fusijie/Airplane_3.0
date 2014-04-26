#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
	gameOverLayer = nullptr;
	score = 0;
}

GameOverScene::~GameOverScene(void)
{

}

GameOverScene* GameOverScene::create(int passScore)
{
	GameOverScene *pRet = new GameOverScene(); 
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

bool GameOverScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());

		gameOverLayer = GameOverLayer::create(score);
		this->addChild(gameOverLayer);

		bRet = true;
	} while (0);
	return bRet;
}
