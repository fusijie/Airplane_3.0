#include "Enemy.h"

Enemy::Enemy(void)
{
	m_sprite = nullptr;
	m_life = 0;
}

Enemy::~Enemy(void)
{
}

Enemy* Enemy::create()
{
	Enemy* pRet = new Enemy;
    if (pRet != nullptr) {
        pRet->autorelease();
        return pRet;
    }
	else
        return nullptr;
}

void Enemy::bindSprite(Sprite* sprite, int life)
{
	m_sprite = sprite;
	m_life = life;
	this->addChild(m_sprite);
}

Sprite* Enemy::getSprite()
{
	return m_sprite;
}

int Enemy::getLife()
{
	return m_life;
}

void Enemy::loseLife()
{
	m_life--;
}

Rect Enemy::getBoundingBox()
{
	auto rect = m_sprite->boundingBox();
	auto pos = this->convertToWorldSpace(rect.origin);
	Rect enemyRect(pos.x,pos.y,rect.size.width,rect.size.height);
	return enemyRect;
}