#pragma once
#include <DxLib.h>
#include <array>
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
		Idle,    // 通常
		DiveMove,// 潜り移動
		Move,    // 移動
		Jump,    // ジャンプ
		Shot,    // 攻撃
		Dive,    // 潜る
		Damage,  // ダメージ
		Death,   // 死
		Respawn, // 復活
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

	// ダメージUIの描画処理
	void DrawDamageUI(int offsetX = 0);

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

	// プレイヤーの位置を設定
	void SetPos(VECTOR pos);

	// プレイヤーのリスポーン位置を設定
	void SetRespawnPos(const VECTOR& pos);

	/// <summary>
	/// プレイヤーのY軸回転角度（ラジアン）を設定する
	/// </summary>
	/// <param name="angleY"></param>
	void SetAngleY(float angleY) { m_angle = angleY; }

	// ダメージを受ける処理
	void ApplyDamage(float damage);

	// リスポーンする処理
	void Respawn();

	// 攻撃中かどうか
	bool IsShooting() const;

	// 潜っているかどうか
	bool IsDiving() const { return m_isDiving; };

	std::vector<std::unique_ptr<Bullet>>& GetBullets();

	int GetColor() const { return m_playerColor; }

private:
	// 最初のプレイヤーの状態をIdleにする
	PlayerState m_state = PlayerState::Idle;

	int m_hp = 100;         // プレイヤーHP
	int m_maxHp = 100;      // プレイヤーの最大HP
	int m_playerColor;      // 各プレイヤーの色
	int m_respawnTimer = 0; // リスポーンタイマー

	Animation m_animation;
	// プレイヤーモデル
	int m_modelHandle;
	// アニメーション
	int m_idleAnim;
	int m_shotAnim;
	int m_runAnim;
	// SE
	int m_shotSE;    // 攻撃中のSE
	int m_walkSE;    // 歩きSE
	int m_inkWalkSE; // インクの上での歩きSE
	int m_deathSE;   // 死んだときの水SE
	int m_hitSE;     // 攻撃が当たった際のSE
	int m_diveSE;    // 潜りSE
	int m_diveMoveSE;// 潜り移動SE

	// 角度
	float m_angle = 0.0f;

	VECTOR m_pos;             // 位置
	VECTOR m_respawnPos;      // リスポーン位置
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

	// ダメージUIの画像ハンドル配列
	std::array<int, 3>m_damageUIHandle = { -1,-1,-1 };
};