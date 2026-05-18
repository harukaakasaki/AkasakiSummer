#include "SceneMain.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"

SceneMain::SceneMain() :
	m_frameCount(0),
	m_timeScale(1.0)
{
	m_pPlayer = new Player();
	m_pCamera = new Camera();
}

SceneMain::~SceneMain()
{

}


void SceneMain::Init()
{
	// カリングの設定（裏面のポリゴンは見えないようにする）
	SetUseBackCulling(true);

	// Zバッファの設定
	SetUseZBuffer3D(true);	// Zバッファを使います
	SetWriteZBuffer3D(true);	// 描画する物体はZバッファにも距離を書き込む

	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 300.0f, -700), VGet(0.0f, 0.0f, 0.0f));
	SetupCamera_Perspective(DX_PI_F / 3.0f);
	SetCameraNearFar(200.0f, 1500.0f);


	m_pPlayer->Init();
	m_pCamera->Init();
}

void SceneMain::Update()
{
	m_frameCount++;
	m_pPlayer->Update(m_pCamera->GetAngle(), m_timeScale);
	m_pCamera->Update(m_pPlayer->GetPos());
}

void SceneMain::Draw()
{
	DrawGrid();
	m_pPlayer->Draw();
	m_pCamera->Draw();

	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
	SetBackgroundColor(180,180,180);
}

void SceneMain::DrawGrid()
{
	// 直線の始点と終点
	VECTOR startPos;
	VECTOR endPos;

	for (int z = -300; z <= 300; z += 100)
	{
		startPos = VGet(-300.0f, 0.0f, static_cast<float>(z));
		endPos = VGet(300.0f, 0.0f, static_cast<float>(z));
		DrawLine3D(startPos, endPos, 0xffffff);
	}
	for (int x = -300; x <= 300; x += 100)
	{
		startPos = VGet(static_cast<float>(x), 0.0f, -300.0f);
		endPos = VGet(static_cast<float>(x), 0.0f, 300.0f);
		DrawLine3D(startPos, endPos, 0xffffff);
	}
}
