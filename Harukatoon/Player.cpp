#include "Player.h"
#include "Pad.h"

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
}

Player::~Player()
{
}

void Player::Init()
{
}
void Player::Update(float cameraAngle,float timeScale)
{
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

}
void Player::Draw()
{
	

	// プレイヤーの描画
	DrawCapsule3D(VGet(m_pos.x+0.0f, m_pos.y+100.0f, m_pos.z+0.0f), VGet(m_pos.x+0.0f, m_pos.y+180.0f, m_pos.z+0.0f), 40.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	DrawCapsule3D(VGet(m_pos.x+50.0f, m_pos.y+165.0f, m_pos.z+0.0f), VGet(m_pos.x+50.0f, m_pos.y+170.0f, m_pos.z+0.0f), 40.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
}

Vector3 Player::GetPos() const
{
	return m_pos;
}

bool Player::IsShooting() const
{
	return isShooting;
}

