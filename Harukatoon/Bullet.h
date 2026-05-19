#pragma once
class Bullet
{
public:
	Bullet();
	~Bullet();

	void Init();
	void Update();
	void Draw();
	// 弾が生きているか
	bool IsBulletAlive()const { return m_isAlive; };

	// 弾を撃っている関数を作る
	// 撃つ→速度 * 重力で放物線を描くように発射できるはず！

private:
	// 自分の位置pos

	// 速度velocity

	// 弾の寿命（寿命の場合距離測れなくない？）

	// 弾が生きているかのフラグ
	bool m_isAlive;

};

