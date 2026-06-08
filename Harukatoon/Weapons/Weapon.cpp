#include "Weapon.h"
#include <DxLib.h>

Weapon::Weapon():
	m_whoShot(0.0f),
	m_bullets(0.0f),
	m_shootTimer(0.0f)
{
}

Weapon::~Weapon()
{
}

void Weapon::Init()
{

}
void Weapon::Update()
{
	for (auto& bullet : m_bullets)
	{
		bullet->Update();

		if (!bullet->IsBulletAlive())
		{
			VECTOR paintPos = bullet->GetPos();
		}

	}
	

}
void Weapon::Draw()
{
	for (auto& bullet : m_bullets)
	{
		bullet->Draw();
	}

}
void Weapon::UseWeapon(VECTOR playerPos,VECTOR shotVel)
{

	m_bullets.push_back(std::make_unique<Bullet>(playerPos, shotVel));

	printfDx("ウェポンで攻撃中！\n");
}
