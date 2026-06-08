#pragma once
#include <DxLib.h>
#include "../Systems/Vector3.h"

class Weapon;
class Bomb;

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update(float cameraAngle,float timeScale);
	void Draw();

	// プレイヤー情報の取得
	VECTOR GetPos() const;// 位置
	bool IsShooting() const;// 攻撃中かどうか


private:
	int m_modelHandle;// モデル

	float m_angle = 0.0f;// 角度

	VECTOR m_pos;// 位置
	VECTOR m_move;// 移動
	bool isShooting = false;// 攻撃しているかどうか

	Weapon* m_pWeapon;// プレイヤーが持つウェポン
	Bomb* m_pBomb;// プレイヤーが持つボム
};

