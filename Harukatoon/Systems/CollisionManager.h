#pragma once
#include <vector>
#include <memory>

class Player;
class Bullet;
class CollisionManager
{
public:

	// コンストラクタ、デストラクタ
	CollisionManager() = default;
	~CollisionManager() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="m_players"></param>
	/// <param name="m_bullets"></param>
	void Update(std::vector<Player*>& m_players,
		        std::vector<std::unique_ptr<Bullet>>& m_bullets);

private:

	/// <summary>
	/// プレイヤーと弾の当たり判定
	/// </summary>
	/// <param name="m_players"></param>
	/// <param name="m_bullets"></param>
	void CheckPlayerBulletCollisions(std::vector<Player*>& m_players,
		                             std::vector<std::unique_ptr<Bullet>>& m_bullets);
};

