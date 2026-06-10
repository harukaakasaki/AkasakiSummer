#include "SceneMain.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>
#include "../GameObjects/Player.h"
#include "../Systems/Camera.h"
#include "../Stages/StageManager.h"

SceneMain::SceneMain() :
	m_frameCount(0),
	m_timeScale(1.0),
	m_gameUI(-1)
{
	m_pStageManager = new StageManager();
	m_pPlayer = new Player(m_pStageManager);
	m_pCamera = new Camera();
}

SceneMain::~SceneMain()
{
	delete m_pPlayer;
	delete m_pCamera;
	delete m_pStageManager;
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

	m_pPlayer->Init();
	m_pCamera->Init();
	m_pStageManager->Init();

	m_gameUI = LoadGraph("data/UI/GameUI_1.png");
	assert(m_gameUI != -1);
}

void SceneMain::Update()
{
	m_frameCount++;
	m_pPlayer->Update(m_pCamera->GetAngle(), m_timeScale);
	m_pCamera->Update(m_pPlayer->GetPos());
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
		m_pStageManager->Paint(paintX, paintZ, 1);
	}
	// もしマウスの右クリックを押したら、テクスチャに緑色が付く
	else if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		m_pStageManager->Paint(paintX, paintZ, 2);
	}
}

void SceneMain::Draw()
{
	DrawGrid();
	m_pPlayer->Draw();
	m_pCamera->Draw();
	m_pStageManager->Draw();

	/*SetUseZBuffer3D(false);

	SetUseZBuffer3D(true);*/
	
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);

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