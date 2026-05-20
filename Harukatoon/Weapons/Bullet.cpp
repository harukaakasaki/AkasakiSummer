#include "Bullet.h"
#include <DxLib.h>

namespace
{
	float kShotSpeed;
}

Bullet::Bullet():
	m_pos(0.0f,0.0f,0.0f),
	m_velocity(0),
	m_isAlive(false)
{

}

Bullet::~Bullet()
{

}

void Bullet::Init()
{

}
void Bullet::Update()
{
	Shot();
}
void Bullet::Draw()
{

}

void Bullet::Shot()
{
	m_isAlive == true;
}


