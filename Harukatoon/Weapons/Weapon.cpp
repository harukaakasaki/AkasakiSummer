#include "Weapon.h"
#include <vector>
#include <DxLib.h>

Weapon::Weapon(StageManager* stageManager):
	m_whoShot(0.0f),
	m_bullets(0.0f),
	m_shootTimer(0.0f)
{
	m_stageManager = stageManager;
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

			m_stageManager->Paint(paintPos.x, paintPos.z, 1);
		}
		// ここから先を消した場合、永遠に塗れる
		else
		{
			aliveBullets.push_back(std::move(bullet));
		}
	}
	m_bullets = std::move(aliveBullets);
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

	//printfDx("ウェポンで攻撃中！\n");
}
