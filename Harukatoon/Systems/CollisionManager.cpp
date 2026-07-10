#include "CollisionManager.h"
#include "../GameObjects/Player.h"
#include "../Weapons/Bullet.h"
#include <DxLib.h>
#include <algorithm>

void CollisionManager::Update(std::vector<Player*>& m_players,
	std::vector<std::unique_ptr<Bullet>>& m_bullets)
{
	checkPlayerBulletCollisions(m_players, m_bullets);
}

void CollisionManager::checkPlayerBulletCollisions(std::vector<Player*>& m_players,
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

			// プレイヤーと弾の色が同じ場合は無視する
			if (player->GetColor() == bullet->GetColor()) continue;

			// プレイヤーと弾の位置や半径を取得する
			VECTOR playerPos = player->GetPos();
			float playerRadius = 0.0f; // プレイヤーの半径

			VECTOR bulletPos = bullet->GetPos();
			float bulletRadius = 10.0f;// 弾の半径

			// プレイヤーが潜っている（イカ状態）場合は球と球の当たり判定
			if (player->IsDiving())
			{
				playerPos.y += 50.0f; // プレイヤーの中心を上にずらす
				playerRadius = 80.0f; // プレイヤーの半径

				// プレイヤーからベクトルを計算
				VECTOR diff = VSub(bulletPos, playerPos);

				// 距離の二乗を計算する
				float distanceSquared = VSquareSize(diff);

				// 半径の合計の二乗を計算する
				float radiusSum = playerRadius + bulletRadius;
				float radiusSumSquared = radiusSum * radiusSum;

				// 衝突判定
				if (distanceSquared <= radiusSumSquared)
				{
					// 衝突処理
					bullet->KillBullet();

#ifdef DEBUG
					printfDx("イカ状態のプレイヤーに弾が当たった！\n");
#endif // DEBUG

					break; // このプレイヤーに対してはもう他の弾をチェックしない
				}
			}
			// プレイヤーが潜っていない（ヒト状態）場合は球と球の当たり判定（後からカプセルと球に変更予定）
			else
			{
				playerPos.y += 150.0f; // プレイヤーの中心を上にずらす
				playerRadius = 90.0f;  // プレイヤーの半径

				// プレイヤーからベクトルを計算
				VECTOR diff = VSub(bulletPos, playerPos);

				// 距離の二乗を計算する
				float distanceSquared = VSquareSize(diff);

				// 半径の合計の二乗を計算する
				float radiusSum = playerRadius + bulletRadius;
				float radiusSumSquared = radiusSum * radiusSum;

				// 衝突判定
				if (distanceSquared <= radiusSumSquared)
				{
					// 衝突処理
					bullet->KillBullet();
#ifdef DEBUG
					printfDx("プレイヤーに弾が当たった！\n");
#endif // DEBUG

					break; // このプレイヤーに対してはもう他の弾をチェックしない
				}
			}
		}
	}
}