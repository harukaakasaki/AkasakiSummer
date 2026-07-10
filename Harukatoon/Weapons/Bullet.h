#pragma once
#include <DxLib.h>
#include "../Systems/Vector3.h"
class Bullet
{
public:
	Bullet(VECTOR startPos, VECTOR startVel,int color);
	// 弾の色を取得する
	int GetColor() const { return m_color; }
	~Bullet();

	void Init();
	// 撃つ→速度 * 重力で放物線を描くように発射できるはず！
	void Update();
	void Draw();

	VECTOR GetPos();
	// 弾が生きているか
	bool IsBulletAlive()const { return m_isAlive; };

	// 弾を消す
	void KillBullet() { m_isAlive = false; };

private:
	// 自分の位置pos
	VECTOR m_pos;

	// 速度velocity
	VECTOR m_velocity;

	// 弾が生きているかのフラグ
	bool m_isAlive;

	// 弾の色
	int m_color;

};

