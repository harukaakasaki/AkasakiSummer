#include "Weapon.h"
#include <vector>
#include <DxLib.h>

namespace
{

}

Weapon::Weapon(StageManager* stageManager,int playerColor) :
	m_stageManager(stageManager),
	m_playerColor(playerColor),
	m_whoShot(0.0f),
	m_bullets(0.0f),
	m_shotTimer(0),
	m_shotInterval(2)// このフレーム内に一発発射する
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
	if (m_shotTimer > 0)
	{
		m_shotTimer--;
	}

	std::vector<std::unique_ptr<Bullet>>nextFrameBullets;

	for (auto& bullet : m_bullets)
	{
		bullet->Update();

		if (bullet->IsBulletAlive())
		{
			// 生きている弾だけを次のフレームへ持っていく
			nextFrameBullets.push_back(std::move(bullet));
		}
		else
		{
			// ステージを塗る
			VECTOR currentPos = bullet->GetPos();

			m_stageManager->Paint(currentPos.x, currentPos.z, m_playerColor, 100);
		}
	}
	m_bullets = std::move(nextFrameBullets);
}
void Weapon::Draw()
{
	for (const auto& bullet : m_bullets)
	{
		bullet->Draw();
	}
}
void Weapon::UseWeapon(VECTOR playerPos,VECTOR shotVel)
{
	//printfDx("ウェポンで攻撃中！\n");
	// x軸、y軸の-0.8～6.0fの間からランダムで弾が飛ぶ
	if (m_shotTimer == 0)
	{
		float rateX = (float)rand() / RAND_MAX;
		float spreadX = -0.8f + rateX * 6.0f;

		float rateY = (float)rand() / RAND_MAX;
		float spreadY = -0.8f + rateY * 3.2f;

		VECTOR randomVel = shotVel;
		randomVel.x += spreadX;
		randomVel.y += spreadY;

		randomVel.y += 21.0f; // 弾の初速を上に上げる(放物線)

		// 弾を生成して管理リストに追加
		m_bullets.push_back(std::make_unique<Bullet>(playerPos, randomVel));

		// 次の弾までそのフレーム待つ
		m_shotTimer = m_shotInterval;

	}
}