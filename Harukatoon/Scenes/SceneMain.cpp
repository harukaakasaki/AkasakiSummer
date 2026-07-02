#include "SceneMain.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>
#include "../GameObjects/Player.h"
#include "../Systems/Camera.h"
#include "../Systems/Pad.h"
#include "../Stages/StageManager.h"

namespace
{
	constexpr int kPlayerOrange = 1;// プレイヤーがオレンジ
	constexpr int kPlayerBlue = 2;  // プレイヤーがブルー
}

SceneMain::SceneMain() :
	m_frameCount(0),
	m_timeScale(1.0),
	m_gameUI(-1)
{
	m_pStageManager = new StageManager();
	m_pPlayer1 = new Player(m_pStageManager, DX_INPUT_PAD1,kPlayerOrange);
	m_pPlayer2 = new Player(m_pStageManager, DX_INPUT_PAD2,kPlayerBlue);
	m_pCamera1 = new Camera();
	m_pCamera2 = new Camera();
}

SceneMain::~SceneMain()
{
	delete m_pStageManager;
	delete m_pPlayer1;
	delete m_pPlayer2;
	delete m_pCamera1;
	delete m_pCamera2;
}


void SceneMain::Init()
{
	// カリングの設定（裏面のポリゴンは見えないようにする）
	SetUseBackCulling(true);

	// Zバッファの設定
	SetUseZBuffer3D(true);	// Zバッファを使います
	SetWriteZBuffer3D(true);	// 描画する物体はZバッファにも距離を書き込む

	SetBackgroundColor(200, 200, 200);

	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 300.0f, -700), VGet(0.0f, 0.0f, 0.0f));
	SetupCamera_Perspective(DX_PI_F / 3.0f);
//	SetCameraNearFar(200.0f, 1500.0f);
	SetCameraNearFar(1.0f, 1500.0f);

	m_pPlayer1->Init();
	m_pPlayer2->SetPos(VGet(0.0f, 0.0f, 0.0f));
	m_pPlayer2->Init();
	m_pPlayer2->SetPos(VGet(200.0f, 0.0f, 0.0f));
	m_pCamera1->Init(DX_INPUT_PAD1);
	m_pCamera2->Init(DX_INPUT_PAD2);
	m_pStageManager->Init();

	m_gameUI = LoadGraph("data/UI/GameUI_1.png");
	assert(m_gameUI != -1);
}

void SceneMain::Update()
{
	Pad::Update();

	m_frameCount++;
	m_pPlayer1->Update(m_pCamera1->GetYaw(),m_pCamera1->GetPitch(), m_timeScale);
	m_pPlayer2->Update(m_pCamera2->GetYaw(),m_pCamera2->GetPitch(), m_timeScale);
	m_pCamera1->Update(m_pPlayer1->GetPos());
	m_pCamera2->Update(m_pPlayer2->GetPos());
	m_pStageManager->Update();
	InkPaint();
}

void SceneMain::InkPaint()
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// マウスの座標を3Dに変換する
	VECTOR rayStart = ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 0.0f));
	VECTOR rayEnd   = ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 1.0f));

	float vecY = rayEnd.y - rayStart.y;

	float paintX = 0.0f;
	float paintZ = 0.0f;

	if (std::abs(vecY) > 0.0001f)
	{
		float t = -rayStart.y / vecY;

		paintX = rayStart.x + t * (rayEnd.x - rayStart.x);
		paintZ = rayStart.z + t * (rayEnd.z - rayStart.z);
	}

	// もしマウスの左クリックを押したら、テクスチャに赤色が付く
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		m_pStageManager->Paint(paintX, paintZ, 1,100.0f);
	}
	// もしマウスの右クリックを押したら、テクスチャに緑色が付く
	else if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		m_pStageManager->Paint(paintX, paintZ, 2,100.0f);
	}
}

void SceneMain::Draw()
{
	// プレイヤー1は描画範囲を左半分にする（x = 640）
	SetDrawArea(0, 0, 640, 720);
	//3Dカメラの描画範囲を左半分に合わせる（320）
	SetCameraScreenCenter(320.0f, 360.0f);

	// プレイヤー1のカメラを描画
	m_pCamera1->Draw();

	// プレイヤー1に映る世界の描画
	DrawGrid();
	m_pPlayer1->Draw();
	m_pPlayer2->Draw();
	m_pStageManager->Draw();
	
	// プレイヤー2は描画範囲を右半分にする（x = 1280）
	SetDrawArea(640, 0, 1280, 720);
	//3Dカメラの描画範囲を右半分に合わせる（320）
	SetCameraScreenCenter(960.0f, 360.0f);
	
	// プレイヤー2のカメラを描画
	m_pCamera2->Draw();

	// プレイヤー2に映る世界の描画
	DrawGrid();
	m_pPlayer1->Draw();
	m_pPlayer2->Draw();
	m_pStageManager->Draw();

	// 描画範囲を元に戻す
	SetDrawArea(0, 0, 1280, 720);
	SetCameraScreenCenter(640.0f, 360.0f);


	/*SetUseZBuffer3D(false);

	SetUseZBuffer3D(true);*/
	
#ifdef DEBUG
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
#endif // DEBUG

	// UIの描画
	int width, height;
	GetGraphSize(m_gameUI, &width, &height);
	
	DrawExtendGraph(270, 10, 100 + width / 4, 100 + height / 5, m_gameUI, true);
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
		DrawLine3D(startPos, endPos, 0x0000ff);
	}
	for (int x = -300; x <= 300; x += 100)
	{
		startPos = VGet(static_cast<float>(x), 0.0f, -300.0f);
		endPos = VGet(static_cast<float>(x), 0.0f, 300.0f);
		DrawLine3D(startPos, endPos, 0x0000ff);
	}
}