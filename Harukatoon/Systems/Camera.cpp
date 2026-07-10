#include "Camera.h"
#include <cmath>
#include <DxLib.h>
#include "Pad.h"
#include "EffekseerForDXLib.h"

Camera::Camera() :
	m_cameraYaw(0.0f),
	m_cameraPitch(0.0f),
	m_cameraPos{ 0.0f,0.0f,0.0f },
	m_cameraTarget{ 0.0f, 0.0f, 0.0f },
	m_skyModelHandle(-1),
	m_padNo(0)
{
}

Camera::~Camera()
{
	MV1DeleteModel(m_skyModelHandle);
}

void Camera::Init(int padNo)
{
	m_padNo = padNo;

	// 空のモデルを読み込む
	m_skyModelHandle = MV1LoadModel("data/Models/Sky.mv1");
	assert(m_skyModelHandle != -1);

	// 空のモデルの大きさを設定
	MV1SetScale(m_skyModelHandle, VGet(5.0f, 5.0f, 5.0f));

	// カメラの描画範囲
	SetCameraNearFar(10.0f, 20000.0f);

	// カメラ初期位置
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 200.0f;
	m_cameraPos.z = -400.0f;

	// 注視点の初期位置
	m_cameraTarget.x = 0.0f;
	m_cameraTarget.y = 100.0f;
	m_cameraTarget.z = 0.0f;

	m_cameraYaw = DX_PI_F / 2;
}
void Camera::Update(VECTOR playerPos)
{
	// 毎フレームカメラにエフェクトを描画する
	// とてもすんごく重要
	Effekseer_Sync3DSetting();

	int camX, camY;
	GetJoypadAnalogInputRight(&camX, &camY, m_padNo);

	float x = camX / -800.0f;
	float y = camY / 1000.0f;

	// 感度
	float sensitivity = 0.05f;

	// 横回転
	m_cameraYaw += x * sensitivity;

	// 縦回転(あとで)
	m_cameraPitch += y * sensitivity;

	// カメラとプレイヤーとの距離
	float distance = 800.0f;
	// カメラの高さ
	float height = 600.0f;

	// カメラズーム（サブウェポンを投げるときに長押しでズームするようにしたい！）
	// そんな時はラープ！
	/*if (Pad::IsPress(PAD_INPUT_6))
	{
		distance = 600.0f;
		height = 50.0f;
	}*/

	// カメラの制限
	float limit = DX_PI_F / 3.0f;
	if (m_cameraPitch > limit)m_cameraPitch = limit;
	if (m_cameraPitch < -limit)m_cameraPitch = -limit;

	// カメラ位置(プレイヤーを中心に回転する)
	m_cameraPos.x = playerPos.x + cosf(m_cameraPitch) * cosf(m_cameraYaw) * distance;
	m_cameraPos.z = playerPos.z + cosf(m_cameraPitch) * sinf(m_cameraYaw) * distance;
	m_cameraPos.y = playerPos.y + sinf(m_cameraPitch) * distance + height;

	m_cameraTarget = playerPos;

	m_cameraTarget.y += 300.0f;
}
void Camera::Draw()
{
	// カメラと注視点を設定
	SetCameraPositionAndTarget_UpVecY(
		VGet(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z),
		VGet(m_cameraTarget.x, m_cameraTarget.y, m_cameraTarget.z));

	// 空を描画
	MV1SetPosition(m_skyModelHandle, VGet(m_cameraTarget.x, m_cameraTarget.y, m_cameraTarget.z));
	MV1DrawModel(m_skyModelHandle);

	// 空を描画する前にZバッファへの書き込みをオフにする
	SetWriteZBuffer3D(false);

	// 空を描画した後にZバッファへの書き込みをオンにする
	SetWriteZBuffer3D(true);
}

