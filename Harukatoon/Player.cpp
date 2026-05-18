#include "Player.h"
#include "Pad.h"

namespace
{
	constexpr float kSpeed = 200.0f;// プレイヤーの移動速度
}

Player::Player():
	m_modelHandle(-1),
	m_angle(0.0f),
	m_pos(0.0f, 0.0f, 0.0f),
	isShooting(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
}
void Player::Update(float cameraAngle,float)
{
	int x, y;
	GetJoypadAnalogInput(&x, &y, DX_INPUT_PAD1);

	float stickX = x / 1000.0f;
	float stickY = y / 1000.0f;

	// デッドゾーン（微妙な入力を無視する）
	float deadZone = 0.2f;
	if (fabs(stickX) < deadZone)stickX = 0.0f;
	if (fabs(stickY) < deadZone)stickY = 0.0f;

	// 移動
	Vector3 move = { 0.0f, 0.0f, 0.0f };

	// 前後
	move.x += cosf(cameraAngle) * stickY;
	move.z += sinf(cameraAngle) * stickY;
	// 左右
	move.x += -sinf(cameraAngle) * stickX;
	move.z += cosf(cameraAngle) * stickX;

	float len = sqrtf(move.x * move.x + move.z * move.z);
	if (len > 0)
	{
		// 移動方向から角度を作る
		m_angle = atan2f(-move.x, -move.z);

		// 正規化
		move.x /= len;
		move.z /= len;

		move.x = move.x * kSpeed;
		move.z = move.z * kSpeed;
	}

	m_pos.x += move.x;
	m_pos.z += move.z;

}
void Player::Draw()
{
	// プレイヤーの描画
	DrawCapsule3D(VGet(0.0f,100.0f,0.0f),VGet(0.0f,180.0f,0.0f),40.0f,8,GetColor(255,255,255),GetColor(255,255,255),TRUE);
}

Vector3 Player::GetPos() const
{
	return Vector3();
}

bool Player::IsShooting() const
{
	return isShooting;
}

