#pragma once
#include <DxLib.h>
#include "../Systems/Animation.h"
#include "../Systems/Vector3.h"
#include "../Stages/StageManager.h"

class Weapon;
class Bomb;

class Player
{
	enum class PlayerState
	{
		Idle,   // 通常
		Run,   // 移動
		Jump,   // ジャンプ
		Shot, // 攻撃
		Dive,   // 潜る
		Damage, // ダメージ
		Death,  // 死
		Respawn,// 復活
	};

public:
	Player(StageManager* stageManager);
	~Player();

	void Init();
	void Update(float cameraAngle,float timeScale);
	void Draw();

	void Jump();

	// プレイヤー情報の取得
	VECTOR GetPos() const;// 位置
	bool IsShooting() const;// 攻撃中かどうか

	// プレイヤー1、2をインスタンス化する


private:
	// 最初のプレイヤーの状態をIdleにする
	PlayerState m_state = PlayerState::Idle;

	Animation m_animation;
	// プレイヤーモデル
	int m_modelHandle;
	// アニメーション
	int m_idleAnim;
	int m_shotAnim;
	int m_runAnim;

	// 角度
	float m_angle = 0.0f;

	VECTOR m_pos;// 位置
	VECTOR m_move;// 移動
	float m_velocityY;// 上に飛ぶ力
	bool m_isGround = false;  // 地面にいるかどうか
	bool m_isShooting = false;// 攻撃しているかどうか

	Weapon* m_pWeapon;// プレイヤーが持つウェポン
	Bomb* m_pBomb;// プレイヤーが持つボム
	// Stageマネージャーのポインタ
	StageManager* m_stageManager;
};

