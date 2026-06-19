#include "Player.h"
#include <DxLib.h>
#include <cmath>
#include "../Systems/Pad.h"
#include "../Weapons/Weapon.h"
#include "../Weapons/Bomb.h"

namespace
{
	constexpr float kSpeed = 15.0f;        // プレイヤーの移動速度
	constexpr float kAttackingSpeed = 5.0f;// プレイヤーの攻撃中の移動速度
	constexpr float kDiveSpeed = 25.0f;    // プレイヤーの潜り移動速度
	constexpr float kShotSpeed = 25.0f;    // 弾速度

	constexpr VECTOR kScale = { 2.0f,2.0f,2.0f };// プレイヤーの大きさ
}

Player::Player(StageManager* stageManager):
	m_modelHandle(-1),
	m_angle(0.0f),
	m_move{0.0f,0.0f,0.0f},
	m_pos{ 0.0f, 0.0f, 0.0f },
	isShooting(false)
{
	m_pWeapon = new Weapon(stageManager);
	m_pBomb = new Bomb();
}

Player::~Player()
{
	delete m_pWeapon;
	delete m_pBomb;
}

void Player::Init()
{
	m_modelHandle = MV1LoadModel("data/Player_Orange_Model.mv1");
	MV1SetScale(m_modelHandle, kScale);// 初期のプレイヤーの大きさ
}
void Player::Update(float cameraAngle,float timeScale)
{
	
	Pad::Update();

	int x, y;
	GetJoypadAnalogInput(&x, &y, DX_INPUT_PAD1);

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

		m_move.x = m_move.x * kSpeed * timeScale;
		m_move.z = m_move.z * kSpeed * timeScale;
	}

	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	// 攻撃
	XINPUT_STATE xinputState;

	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);
	bool isWeaponPress = (xinputState.RightTrigger > 128);// RTが押された
	// 攻撃中はプレイヤーのスピードが遅くなるようにしたい
	if (isWeaponPress)
	{
		m_move.x = m_move.x * kAttackingSpeed * timeScale;
		m_move.z = m_move.z * kAttackingSpeed * timeScale;
		
	}
	bool isDivePress = (xinputState.LeftTrigger > 128);   // LTが押された
	bool isBombPress = Pad::IsPress(PAD_INPUT_6);         // RBが押された

	// 入力情報は優先度をつけて管理する
	if (isWeaponPress)
	{
		VECTOR weaponPos = VGet(m_pos.x, m_pos.y + 300.0f, m_pos.z);

		float speed = kShotSpeed;

		VECTOR shotVelocity;
		shotVelocity.x = -cosf(cameraAngle) * speed;
		shotVelocity.y = 2.0f;
		shotVelocity.z = -sinf(cameraAngle) * speed;


		m_pWeapon->UseWeapon(weaponPos,shotVelocity);
		
		isShooting = true;
	}
	else
	{
		isShooting = false;
	}
	if (isBombPress)
	{
		m_pBomb->Throw();
	}
	else if (isDivePress)
	{
		printfDx("潜ってる～\n");
	}

	if (isShooting)
	{
		// カメラ方向を見ながらインクを撃つ
		// atan2fでcos,sinのカメラアングルを合わせる
		m_angle = atan2f(cosf(cameraAngle),sinf(cameraAngle));
	}

	m_pWeapon->Update();

}
void Player::Draw()
{
	m_pWeapon->Draw();

	// プレイヤーの描画
	int playerCapsule = DrawCapsule3D(VGet(m_pos.x+0.0f, m_pos.y+100.0f, m_pos.z+0.0f), 
		VGet(m_pos.x+0.0f, m_pos.y+180.0f, m_pos.z+0.0f), 
		40.0f, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), false);

	// プレイヤーモデルの回転
	MATRIX rot = MGetRotY(m_angle);// 向き
	MATRIX scale = MGetScale(kScale);// 大きさ
	MATRIX trans = MGetTranslate(VGet(m_pos.x, m_pos.y, m_pos.z));// 動き
	MATRIX mtx = MMult(rot, scale);// 合成
	mtx = MMult(mtx, trans);

	MV1SetMatrix(m_modelHandle, mtx);// モデルハンドルと合わせる

	

	MV1DrawModel(m_modelHandle);// プレイヤー表示
}

VECTOR Player::GetPos() const
{
	return m_pos;
}

bool Player::IsShooting() const
{
	return isShooting;
}

