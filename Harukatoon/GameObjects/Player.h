#pragma once
#include <DxLib.h>
#include <memory>
#include "../Systems/Animation.h"
#include "../Systems/Vector3.h"
#include "../Stages/StageManager.h"

class Weapon;
class Bullet;
class Bomb;

class Player
{
	// プレイヤーの状態の列挙型
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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stageManager"></param>
	/// <param name="padNo">コントローラー番号</param>
	/// <param name="playerColor">プレイヤーのインクの色</param>
	Player(StageManager* stageManager, int padNo, int playerColor);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="cameraAngle">カメラの横回転</param>
	/// <param name="cameraPitch">カメラの縦回転</param>
	/// <param name="timeScale">プレイヤー自体のスピード</param>
	void Update(float cameraYaw, float cameraPitch, float timeScale);

	// 描画処理
	void Draw();

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	void Jump();

	// プレイヤー情報の取得
	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns></returns>
	VECTOR GetPos() const;

	// プレイヤーのポジションを設定
	void SetPos(VECTOR pos);

	// 攻撃中かどうか
	bool IsShooting() const;

	// ダメージを受ける処理
	void ApplyDamage(float damage);

	// 潜っているかどうか
	bool IsDiving() const { return m_isDiving; };

	std::vector<std::unique_ptr<Bullet>>& GetBullets();

	int GetColor() const { return m_playerColor; }

private:
	// 最初のプレイヤーの状態をIdleにする
	PlayerState m_state = PlayerState::Idle;

	int m_hp = 100;    // プレイヤーHP
	int m_maxHp = 100; // プレイヤーの最大HP
	int m_playerColor; // 各プレイヤーの色

	Animation m_animation;
	// プレイヤーモデル
	int m_modelHandle;
	// アニメーション
	int m_idleAnim;
	int m_shotAnim;
	int m_runAnim;

	// 角度
	float m_angle = 0.0f;

	VECTOR m_pos;             // 位置
	VECTOR m_move;            // 移動
	int m_padNo;              // コントローラーの番号
	float m_velocityY;        // 上に飛ぶ力
	bool m_isGround = false;  // 地面にいるかどうか
	bool m_isShooting = false;// 攻撃しているかどうか
	bool m_isDiving = false;  // 潜っているかどうか

	// ウェポンとボムのポインタ
	std::unique_ptr<Weapon> m_pWeapon;
	std::unique_ptr<Bomb> m_pBomb;
	// Stageマネージャーのポインタ
	StageManager* m_pStageManager;
};