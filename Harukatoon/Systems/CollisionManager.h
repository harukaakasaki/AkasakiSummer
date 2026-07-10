#pragma once
#include <vector>
#include <memory>

class Player;
class Bullet;
class CollisionManager
{
public:

	CollisionManager() = default;
	~CollisionManager() = default;

	void Update(std::vector<Player*>& m_players,
		        std::vector<std::unique_ptr<Bullet>>& m_bullets);

private:

	void checkPlayerBulletCollisions(std::vector<Player*>& m_players,
		                             std::vector<std::unique_ptr<Bullet>>& m_bullets);
};

