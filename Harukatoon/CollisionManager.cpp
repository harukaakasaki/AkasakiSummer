#include "CollisionManager.h"
#include "GameObjects/Player.h"
#include "Weapons/Bullet.h"
#include <DxLib.h>
#include <algorithm>

void CollisionManager::Update(std::vector<std::shared_ptr<Player>>& m_players, 
	                          std::vector<std::unique_ptr<Bullet>>& m_bullets)
{
	checkPlayerBulletCollisions(m_players, m_bullets);
}

void CollisionManager::checkPlayerBulletCollisions(std::vector<std::shared_ptr<Player>>& m_players, 
	                                               std::vector<std::unique_ptr<Bullet>>& m_bullets)
{
	// プレイヤーをループする
	for (auto& player : m_players)
	{
		// 弾をループする
		for (auto& bullet : m_bullets)
		{
			// 消滅している弾は無視する
			if (!bullet->IsBulletAlive()) continue;

			// プレイヤーと弾の位置や半径を取得する
			VECTOR bulletPos = bullet->GetPos();
			float bulletRadius = 10.0f;

			// プレイヤーが潜っている（イカ状態）場合は球と球の当たり判定

			// プレイヤーが潜っていない（ヒト状態）場合はカプセルと球の当たり判定



		}
	}
}
