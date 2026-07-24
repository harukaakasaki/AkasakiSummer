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

	constexpr float kAnalogInputScale = 1000.0f;        // スティック入力を正規化するために割る
	constexpr float kDeadZone = 0.2f;                   // スティックのデッドゾーン
	constexpr float kTrigger = 128;                     // アナログトリガーの判定
	constexpr float kRespawnFrame = 180;                // リスポーンまでの時間
	constexpr float kGlobalAnimframe = 0.7f;

	// 当たり判定に関連したマジックナンバー
	constexpr float kDiveColOffsetY = 25.0f;            // 潜り状態のYオフセット
	constexpr float kDiveColRadius = 60.0f;             // 潜り判定の球の半径
	constexpr float kDiveHitRadius = 80.0f;             // 潜り状態の球の当たり判定半径
	constexpr float kCapsuleBottomOffsetY = 40.0f;      // カプセルの下端のオフセット
	constexpr float kCapsuleTopOffsetY = 230.0f;        // カプセルの上端のオフセット
	constexpr float kCapsuleRadius = 40.0f;             // 立体カプセルの半径
	constexpr float kBodyHitBoxOffsetY = 150.0f;        // プレイヤーの球のオフセット
	constexpr float kBodyHitBoxRadius = 90.0f;          // プレイヤーの球の半径
	constexpr int kMeshDivision = 16;                   // 球を描画する際の分割数
	constexpr int kCapsuleDivision = 8;                 // カプセルを描画する際の分割数
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

	float stickX = x / kAnalogInputScale;
	float stickY = y / kAnalogInputScale;

	// デッドゾーン（微妙な入力を無視する）
	float deadZone = kDeadZone;
	if (fabs(stickX) < deadZone)stickX = 0.0f;
	if (fabs(stickY) < deadZone)stickY = 0.0f;

	m_move = { 0.0f,0.0f,0.0f };

	// 移動ベクトルの計算
	// カメラの水平方向を基準にしてスティックを3D座標に変換する
	// 前後
	m_move.x += cosf(cameraAngle) * stickY;
	m_move.z += sinf(cameraAngle) * stickY;
	// 左右
	m_move.x += -sinf(cameraAngle) * stickX;
	m_move.z += cosf(cameraAngle) * stickX;

	float len = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);
	if (len > 0)
	{
		// 移動ベクトルから角度を作る
		m_angle = atan2f(-m_move.x, -m_move.z);

		// 移動ベクトルを正規化する
		m_move.x /= len;
		m_move.z /= len;
	}

	// 入力
	XINPUT_STATE xinputState;
	GetJoypadXInputState(m_padNo, &xinputState);
	bool isWeaponPress = (xinputState.RightTrigger > kTrigger);// RTが押された
	bool isDivePress = (xinputState.LeftTrigger > kTrigger);   // LTが押された
	bool isBombPress = Pad::IsPress(m_padNo, PAD_INPUT_6);     // RBが押された

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
		if (m_state != PlayerState::Move)
		{
			m_animation.Play(m_runAnim, true, kAnimSpeed);
			m_state = PlayerState::Move;
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

		float shotSpeed = kShotSpeed;

		// 撃っている向き
		VECTOR shotVelocity;
		shotVelocity.x = -cosf(cameraPitch) * cosf(cameraAngle) * shotSpeed;
		shotVelocity.z = -cosf(cameraPitch) * sinf(cameraAngle) * shotSpeed;
		shotVelocity.y = -sinf(cameraPitch) * shotSpeed;

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
		// カメラの方向を見ながらインクを撃つ
		// atan2fでcos,sinのカメラアングルを合わせる
		m_angle = atan2f(cosf(cameraAngle), sinf(cameraAngle));
	}

	/*プレイヤーモデルの回転*/
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

	m_pWeapon->Update();
	Jump();
	// アニメーションの再生速度
	m_animation.SetGlobalSpeed(kGlobalAnimframe* timeScale);
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
		DrawSphere3D(VGet(m_pos.x, m_pos.y + kDiveColOffsetY, m_pos.z),
			kDiveColRadius, kMeshDivision,
			::GetColor(255, 125, 0),
			::GetColor(255, 125, 0),
			true);
#ifdef _DEBUG
		DrawSphere3D(VGet(m_pos.x, m_pos.y + kDiveColOffsetY, m_pos.z),
			kDiveHitRadius, kMeshDivision,
			::GetColor(0, 255, 0),
			::GetColor(0, 255, 0),
			false);
#endif // DEBUG

		return;
	}

	// プレイヤーの当たり判定の描画(現在は球)
#ifdef _DEBUG
	int playerCapsule = DrawCapsule3D(VGet(m_pos.x + 0.0f, m_pos.y + kCapsuleBottomOffsetY, m_pos.z + 0.0f),
		VGet(m_pos.x + 0.0f, m_pos.y + kCapsuleTopOffsetY, m_pos.z + 0.0f),
		kCapsuleRadius, kCapsuleDivision, ::GetColor(0, 255, 0), ::GetColor(255, 255, 255), false);
	DrawSphere3D(VGet(m_pos.x, m_pos.y + kBodyHitBoxOffsetY, m_pos.z),
		kBodyHitBoxRadius, kMeshDivision,
		::GetColor(0, 255, 0),
		::GetColor(0, 255, 0),
		false);
#endif

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
		m_respawnTimer = kRespawnFrame; // リスポーン時間
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