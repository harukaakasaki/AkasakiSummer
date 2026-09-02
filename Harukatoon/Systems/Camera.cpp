#include "Camera.h"
#include <cmath>
#include <DxLib.h>
#include <algorithm>
#include "Pad.h"
#include "EffekseerForDXLib.h"

namespace
{
	// カメラが壁に当たらないようにする
	constexpr float kCameraMargin = 20.0f;
}

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
}
void Camera::Update(VECTOR playerPos,int stageModelHandle)
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

	// カメラの制限
	float limit = DX_PI_F / 3.0f;
	m_cameraPitch = std::clamp(m_cameraPitch, -limit, limit);

	// カメラとプレイヤーとの距離
	float distance = 800.0f;
	// カメラの高さ
	float height = 600.0f;

	// ステージとカメラの当たり判定
	VECTOR cameraPos;
	cameraPos.x = playerPos.x + cosf(m_cameraPitch) * cosf(m_cameraYaw) * distance;
	cameraPos.z = playerPos.z + cosf(m_cameraPitch) * sinf(m_cameraYaw) * distance;
	cameraPos.y = playerPos.y + sinf(m_cameraPitch) * distance + height;

	m_cameraTarget = playerPos;
	m_cameraTarget.y += 300.0f;

	m_cameraPos = cameraPos;

	if (stageModelHandle != -1)
	{
		MV1_COLL_RESULT_POLY hitResult = MV1CollCheck_Line(
			stageModelHandle,
			-1,
			m_cameraTarget,
			cameraPos);
		// 壁に当たっていた場合
		if (hitResult.HitFlag == 1)
		{
			VECTOR dir = VSub(hitResult.HitPosition, m_cameraTarget);
			float hitDistance = VSize(dir);

			if (hitDistance > 0.001f)
			{
				// 正規化
				dir = VScale(dir, 1.0f / hitDistance);
				// 注視点側に引き戻す
				float safeDistance = (hitDistance > kCameraMargin) ? (hitDistance - kCameraMargin) : 0.0f;
				// 実際のカメラ位置を壁に手前に設定
				m_cameraPos = VAdd(m_cameraTarget, VScale(dir, safeDistance));
			}
		}
	}
}
void Camera::Draw()
{
	// カメラと注視点を設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// 空を描画
	MV1SetPosition(m_skyModelHandle, m_cameraTarget);

	// 空を描画する前にZバッファへの書き込みをオフにする
	SetWriteZBuffer3D(false);
	MV1DrawModel(m_skyModelHandle);
	// 空を描画した後にZバッファへの書き込みをオンにする
	SetWriteZBuffer3D(true);
}