#include "Player.h"
#include <DxLib.h>
#include <cmath>
#include "../Systems/Pad.h"
#include "../Weapons/Weapon.h"
#include "../Weapons/Bomb.h"

namespace
{
	// アニメーションのインデックス
	constexpr const char* kIdleAnim = "root|Idle";      // 待機アニメーション
	constexpr const char* kShotAnim = "root|Shot";      // 射撃アニメーション
	constexpr const char* kRunAnim = "root|Run";	    // 走るアニメーション

	constexpr float kAnimSpeed = 1.0f;                  // アニメーションスピード
	constexpr float kSpeed = 18.0f;                     // プレイヤーの移動速度
	constexpr float kAttackingSpeed = 13.0f;            // プレイヤーの攻撃中の移動速度
	constexpr float kDiveSpeed = 30.0f;                 // プレイヤーの潜り移動速度
	constexpr float kShotSpeed = 30.0f;                 // 弾速度
	constexpr float kGravity = 0.8f;                    // 重力
	constexpr float kJumpPower = 20.0f;                 // ジャンプ力
	constexpr float kWeaponPosY = 170.0f;               // ウェポンのy軸の位置
	constexpr VECTOR kScale = { 2.0f,2.0f,2.0f };       // プレイヤーの大きさ
}

Player::Player(StageManager* stageManager, int padNo, int playerColor) :
	m_pStageManager(stageManager),
	m_modelHandle(-1),
	m_angle(0.0f),
	m_move{ 0.0f,0.0f,0.0f },
	m_pos{ 0.0f, 0.0f, 0.0f },
	m_respawnPos{ 0.0f, 0.0f, 0.0f },
	m_isShooting(false),
	m_velocityY(0.0f),
	m_idleAnim(-1),
	m_runAnim(-1),
	m_shotAnim(-1),
	m_respawnTimer(0),
	m_padNo(padNo),
	m_playerColor(playerColor)
{
	m_pWeapon = std::make_unique<Weapon>(stageManager, playerColor);
	m_pBomb = std::make_unique<Bomb>();
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
	m_modelHandle = MV1LoadModel("data/Models/Player.mv1");
	m_animation.Init(m_modelHandle);

	m_idleAnim = MV1GetAnimIndex(m_modelHandle, kIdleAnim);
	m_shotAnim = MV1GetAnimIndex(m_modelHandle, kShotAnim);
	m_runAnim = MV1GetAnimIndex(m_modelHandle, kRunAnim);

	// 初期のプレイヤーの大きさ
	MV1SetScale(m_modelHandle, kScale);

	m_animation.Play(m_idleAnim, true, kAnimSpeed);
	m_state = PlayerState::Idle;
}
void Player::Update(float cameraAngle, float cameraPitch, float timeScale)
{
	// 死亡処理
	if (m_state == PlayerState::Death)
	{
		m_respawnTimer--;

		if (m_respawnTimer <= 0)
		{
			Respawn();
		}
		return;
	}

	int x, y;
	// プレイヤー
	GetJoypadAnalogInput(&x, &y, m_padNo);

	float stickX = x / 1000.0f;
	float stickY = y / 1000.0f;

	// デッドゾーン（微妙な入力を無視する）
	float deadZone = 0.2f;
	if (fabs(stickX) < deadZone)stickX = 0.0f;
	if (fabs(stickY) < deadZone)stickY = 0.0f;

	m_move = { 0.0f,0.0f,0.0f };

	// 前後
	m_move.x += cosf(cameraAngle) * stickY;
	m_move.z += sinf(cameraAngle) * stickY;
	// 左右
	m_move.x += -sinf(cameraAngle) * stickX;
	m_move.z += cosf(cameraAngle) * stickX;

	float len = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);
	if (len > 0)
	{
		// 移動方向から角度を作る
		m_angle = atan2f(-m_move.x, -m_move.z);

		// 正規化
		m_move.x /= len;
		m_move.z /= len;
	}

	// 攻撃
	XINPUT_STATE xinputState;

	GetJoypadXInputState(m_padNo, &xinputState);
	bool isWeaponPress = (xinputState.RightTrigger > 128);// RTが押された
	bool isDivePress = (xinputState.LeftTrigger > 128);   // LTが押された
	bool isBombPress = Pad::IsPress(m_padNo, PAD_INPUT_6);// RBが押された

	float speed = kSpeed;

	if (isDivePress)
	{
		m_isDiving = true;

		speed = kDiveSpeed;

#ifdef _DEBUG
		printfDx("潜ってる～\n");
#endif // DEBUG
	}
	else
	{
		m_isDiving = false;
	}

	// 攻撃中はプレイヤーのスピードが遅くなるようにしたい
	if (isWeaponPress)
	{
		if (m_state != PlayerState::Shot)
		{
			m_animation.Play(m_shotAnim, true, kAnimSpeed);
			m_state = PlayerState::Shot;
		}
		m_isShooting = true;

		// 攻撃中は移動速度を遅くする
		speed = kAttackingSpeed;
	}

	else if (len > 0)
	{
		if (m_state != PlayerState::Run)
		{
			m_animation.Play(m_runAnim, true, kAnimSpeed);
			m_state = PlayerState::Run;
		}
	}
	else
	{
		if (m_state != PlayerState::Idle)
		{
			m_animation.Play(m_idleAnim, true, kAnimSpeed);
			m_state = PlayerState::Idle;
		}
	}

	m_move.x = m_move.x * speed * timeScale;
	m_move.z = m_move.z * speed * timeScale;

	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	// 入力情報は優先度をつけて管理する
	if (!m_isDiving && isWeaponPress)
	{
		VECTOR weaponPos = VGet(m_pos.x, m_pos.y + kWeaponPosY, m_pos.z);

		float speed = kShotSpeed;

		// 撃っている向き
		VECTOR shotVelocity;
		shotVelocity.x = -cosf(cameraPitch) * cosf(cameraAngle) * speed;
		shotVelocity.z = -cosf(cameraPitch) * sinf(cameraAngle) * speed;
		shotVelocity.y = -sinf(cameraPitch) * speed;


		m_pWeapon->UseWeapon(weaponPos, shotVelocity);

		m_isShooting = true;
	}
	else
	{
		m_isShooting = false;
	}
	if (isBombPress)
	{
		m_pBomb->Throw();
	}


	if (m_isShooting)
	{
		// カメラ方向を見ながらインクを撃つ
		// atan2fでcos,sinのカメラアングルを合わせる
		m_angle = atan2f(cosf(cameraAngle), sinf(cameraAngle));
	}

	m_pWeapon->Update();
	Jump();
	// アニメーションの再生速度
	m_animation.SetGlobalSpeed(0.7f * timeScale);
	m_animation.Update();
}
void Player::Draw()
{
	if (m_state == PlayerState::Death)
	{
		return;
	}

	m_pWeapon->Draw();

	// 潜り状態のプレイヤーの当たり判定(球)
	if (m_isDiving)
	{
		DrawSphere3D(VGet(m_pos.x, m_pos.y + 25, m_pos.z),
			60, 16,
			::GetColor(255, 125, 0),
			::GetColor(255, 125, 0),
			true);
#ifdef _DEBUG
		DrawSphere3D(VGet(m_pos.x, m_pos.y + 25, m_pos.z),
			80, 16,
			::GetColor(0, 255, 0),
			::GetColor(0, 255, 0),
			false);
#endif // DEBUG

		return;
	}

	// プレイヤーの当たり判定の描画(カプセル)
#ifdef _DEBUG
	int playerCapsule = DrawCapsule3D(VGet(m_pos.x + 0.0f, m_pos.y + 40.0f, m_pos.z + 0.0f),
		VGet(m_pos.x + 0.0f, m_pos.y + 230.0f, m_pos.z + 0.0f),
		40.0f, 8, ::GetColor(0, 255, 0), ::GetColor(255, 255, 255), false);
	DrawSphere3D(VGet(m_pos.x, m_pos.y + 150, m_pos.z),
		90, 16,
		::GetColor(0, 255, 0),
		::GetColor(0, 255, 0),
		false);
#endif

	// プレイヤーモデルの回転

	// 向き
	MATRIX rot = MGetRotY(m_angle);

	// 大きさ
	MATRIX scale = MGetScale(kScale);

	// 動き
	MATRIX trans = MGetTranslate(VGet(m_pos.x, m_pos.y, m_pos.z));

	// 合成
	MATRIX mtx = MMult(rot, scale);

	// 合成
	mtx = MMult(mtx, trans);

	// モデルハンドルと合わせる
	MV1SetMatrix(m_modelHandle, mtx);

	// プレイヤー表示
	MV1DrawModel(m_modelHandle);
}

void Player::Jump()
{
	// A(ジャンプボタン)が押された
	if (m_isGround && Pad::IsTrigger(m_padNo, PAD_INPUT_1))
	{
		m_velocityY = kJumpPower;
		m_isGround = false;
	}
	// 重力
	m_velocityY -= kGravity;

	// 位置更新
	m_pos.y += m_velocityY;
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_velocityY = 0.0f;
		m_isGround = true;
	}
}

VECTOR Player::GetPos() const
{
	return m_pos;
}

void Player::SetPos(VECTOR pos)
{
	m_pos = pos;
}

void Player::SetRespawnPos(const VECTOR& pos)
{
	m_respawnPos = pos;
	m_pos = pos;
}

bool Player::IsShooting() const
{
	return m_isShooting;
}

void Player::ApplyDamage(float damage)
{
	// 受け取ったダメージ分だけHPを減らす
	m_hp -= static_cast<int>(damage);

	if (m_hp < 0)m_hp = 0;

#ifdef _DEBUG
	printfDx("プレイヤー[%d] 現在のHP : %d / %d:HP\n", m_padNo, m_hp, m_maxHp);
#endif // _DEBUG

	// HPが0になった場合、プレイヤーは初期位置に戻る
	if (m_hp <= 0&&m_state!=PlayerState::Death)
	{
		m_state = PlayerState::Death;
		m_respawnTimer = 180; // 3秒間のリスポーン時間
	}
}

void Player::Respawn()
{
	m_pos = m_respawnPos;

	m_hp = m_maxHp;

	m_state = PlayerState::Idle;
}

std::vector<std::unique_ptr<Bullet>>& Player::GetBullets()
{
	return m_pWeapon->GetBullets();
}