#pragma once
#include <DxLib.h>
#include "Vector3.h"
class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update(float cameraAngle,float timeScale);
	void Draw();

		// プレイヤー情報の取得
		Vector3 GetPos() const;// 位置
		bool IsShooting() const;// 攻撃中かどうか


	private:
		int m_modelHandle;// モデル

		float m_angle = 0.0f;// 角度

		Vector3 m_pos;// 位置
		bool isShooting = false;// 攻撃しているかどうか
};

