#pragma once
#include <DxLib.h>
#include "../Systems/Vector3.h"
class Bullet
{
public:
	// コンストラクタ
	Bullet(VECTOR startPos, VECTOR startVel, int color);

	// 弾の色を取得する
	int GetColor() const { return m_color; }

	// デストラクタ
	~Bullet();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾の位置
	/// </summary>
	/// <returns></returns>
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