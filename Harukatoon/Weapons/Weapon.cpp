#include "Weapon.h"
#include <vector>
#include <DxLib.h>
#include <algorithm>

namespace
{
	constexpr int kShotInterval = 2; // このフレーム内に一発発射する
	constexpr float kShotupPower = 18.0f; // 弾が上に飛ぶか下に飛ぶかの初速
	constexpr int kStreatShoothingFrame = 30; // 初めのこのフレームは弾がばらける
}

Weapon::Weapon(StageManager* stageManager,int playerColor) :
	m_stageManager(stageManager),
	m_playerColor(playerColor),
	m_whoShot(0.0f),
	m_bullets(0.0f),
	m_shotTimer(0),
	m_shootingCountFrame(0),
	m_shotInterval(kShotInterval)// このフレーム内に一発発射する
{
	m_stageManager = stageManager;
	m_isMainBullet = false;
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

	bool isAllInactive = true;	//弾が一つも撃たれていないとき
	for (auto& bullet : m_bullets)
	{
		bullet->Update();

		if (bullet->IsBulletAlive())
		{
			// 生きている弾だけを次のフレームへ持っていく
			nextFrameBullets.push_back(std::move(bullet));

			isAllInactive = false;
		}
		else
		{
			// ステージを塗る
			VECTOR currentPos = bullet->GetPos();

			m_stageManager->Paint(currentPos.x, currentPos.z, m_playerColor, 100);
		}
	}
	m_bullets = std::move(nextFrameBullets);

	//弾が一つも撃たれていなければばらけるフレームをリセット
	if (isAllInactive)
	{
		m_shootingCountFrame = 0;
	}
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
	if (m_shootingCountFrame < kStreatShoothingFrame)
	{
		m_shootingCountFrame++;
	}
	

	//printfDx("ウェポンで攻撃中！\n");
	// x軸、y軸の-0.8～6.0fの間からランダムで弾が飛ぶ
	if (m_shotTimer == 0)
	{
		float tempSpeed = VSize(shotVel);

		// -1～1の範囲でランダムな値を生成
		int temp = GetRand(512) - 256;
		float rate = (float)temp / 256.0f;

		VECTOR shotOffsetVel = VCross(shotVel, { 0.0f, 1.0f, 0.0f });

		//　＊この数字を0に近づけるとシャープマーカーで1にするほどモデラー
		float shotWidth = 0.1f * (m_shootingCountFrame/ kStreatShoothingFrame);

		shotOffsetVel = VScale(shotOffsetVel, rate * shotWidth);


		VECTOR randomVel = VNorm(VAdd(shotVel, shotOffsetVel));
		randomVel = VScale(randomVel, tempSpeed);
		
		//弾が上に飛ぶか下に飛ぶか
		float upPower = kShotupPower;

		if (!m_isMainBullet)
		{
			// -1～1の範囲でランダムな値を生成
			int temp2 = GetRand(512) - 256;
			float rate2 = (float)temp2 / 256.0f;
			upPower *= rate2; // 上下のランダムな値を生成
			upPower = std::clamp(upPower, -kShotupPower, kShotupPower); // 上下のランダムな値を制限
		}
		m_isMainBullet = !m_isMainBullet;

		randomVel.y += upPower; // 弾の初速を上に上げる(放物線)

		// 弾を生成して管理リストに追加
		m_bullets.push_back(std::make_unique<Bullet>(playerPos, randomVel));

		// 次の弾までそのフレーム待つ
		m_shotTimer = m_shotInterval;

	}
}