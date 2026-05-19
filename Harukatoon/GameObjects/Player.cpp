#include "Player.h"
#include <DxLib.h>
#include "../Systems/Pad.h"
#include "../Weapons/Weapon.h"
#include "../Weapons/Bomb.h"

namespace
{
	constexpr float kSpeed = 20.0f;// プレイヤーの移動速度
}

Player::Player():
	m_modelHandle(-1),
	m_angle(0.0f),
	m_move{0.0f,0.0f,0.0f},
	m_pos(0.0f, 0.0f, 0.0f),
	isShooting(false)
{
	m_pWeapon = new Weapon();
	m_pBomb = new Bomb();
}

Player::~Player()
{
	delete m_pWeapon;
	delete m_pBomb;
}

void Player::Init()
{
	m_modelHandle = MV1LoadModel("data/player_Gun.mv1");
	
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
	bool isBombPress = Pad::IsPress(PAD_INPUT_6);         // RBが押された

	if (isWeaponPress)
	{
		m_pWeapon->Shot();
		
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

	if (isShooting)
	{
		// カメラ方向を見ながらインクを撃つ
		m_angle = cameraAngle;
	}

}
void Player::Draw()
{
	// プレイヤーの描画
	int playerCapsule = DrawCapsule3D(VGet(m_pos.x+0.0f, m_pos.y+100.0f, m_pos.z+0.0f), 
		VGet(m_pos.x+0.0f, m_pos.y+180.0f, m_pos.z+0.0f), 
		40.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);

	// プレイヤーモデルの回転
	MATRIX rot = MGetRotY(m_angle);// 向き
	MATRIX trans = MGetTranslate(VGet(m_pos.x, m_pos.y, m_pos.z));// 動き
	MATRIX mtx = MMult(rot, trans);// 合成

	MV1SetMatrix(m_modelHandle, mtx);// モデルハンドルと合わせる

	MV1DrawModel(m_modelHandle);// プレイヤー表示
}

Vector3 Player::GetPos() const
{
	return m_pos;
}

bool Player::IsShooting() const
{
	return isShooting;
}

