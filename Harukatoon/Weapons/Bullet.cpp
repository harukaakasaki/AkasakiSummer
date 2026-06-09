#include "Bullet.h"

namespace
{
}

Bullet::Bullet(VECTOR startPos, VECTOR startVel):
	m_pos(startPos),
	m_velocity(startVel),
	m_isAlive(true)
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
	// 位置を動かす
	m_pos = VAdd(m_pos, m_velocity);
	// 弾に重力を掛ける
	m_velocity.y -= 0.3f;
	// 床(今は0.0fが床)に当たった場合、弾は消える（m_isAliveの消滅）
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_isAlive = false;
	}
}
void Bullet::Draw()
{
	// 弾が存在している間のみ描画する
	if (!m_isAlive)return;

	// 緑の弾を表示
	DrawSphere3D(m_pos, 10.0f, 8, GetColor(255,0, 0), GetColor(255, 0, 0),true);
}

VECTOR Bullet::GetPos()
{
	return m_pos;
}


