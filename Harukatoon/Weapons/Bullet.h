#pragma once
#include <DxLib.h>
#include "../Systems/Vector3.h"
class Bullet
{
public:
	Bullet(VECTOR startPos, VECTOR startVel);
	~Bullet();

	void Init();
	void Update();
	void Draw();
	// 弾が生きているか
	bool IsBulletAlive()const { return m_isAlive; };

	// 弾を撃っている関数を作る
	// 撃つ→速度 * 重力で放物線を描くように発射できるはず！
	void Shot();

private:
	// 自分の位置pos
	VECTOR m_pos;


	// 速度velocity
	VECTOR m_velocity;

	// 弾が生きているかのフラグ
	bool m_isAlive;

};

