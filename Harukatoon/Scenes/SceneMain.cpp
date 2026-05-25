#include "SceneMain.h"
#include "DxLib.h"
#include "../GameObjects/Player.h"
#include "../Systems/Camera.h"

SceneMain::SceneMain() :
	m_frameCount(0),
	m_timeScale(1.0),
	m_inkTexture(-1)
{
	m_inkTexture = MakeScreen(500, 500, false);
	m_pPlayer = new Player();
	m_pCamera = new Camera();

	SetRenderTargetToShader(m_inkTexture, FALSE);
	DrawBox(0, 0, 500, 500, GetColor(0, 0, 0), TRUE);
	SetRenderTargetToShader(DX_SCREEN_BACK, FALSE);
}

SceneMain::~SceneMain()
{
	delete m_pPlayer;
	delete m_pCamera;
}


void SceneMain::Init()
{
	// カリングの設定（裏面のポリゴンは見えないようにする）
	SetUseBackCulling(true);

	// Zバッファの設定
	SetUseZBuffer3D(true);	// Zバッファを使います
	SetWriteZBuffer3D(true);	// 描画する物体はZバッファにも距離を書き込む

	SetBackgroundColor(180, 180, 180);

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
	InkPaint();
}

void SceneMain::InkPaint()
{
	
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// もしマウスの左クリックを押したら、テクスチャに色が付く
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		if (mouseX >= 0 && mouseX < 500 && mouseY >= 0 && mouseY < 500)
		{
			// 描画先をm_inkTextureに設定
			SetRenderTargetToShader(m_inkTexture, false);

			// 塗れる範囲
			SetDrawArea(0, 0, 500, 500);

			// 3Dの書き込みをオフにする
			SetWriteZBuffer3D(false);

			// 一時的にブレンドなしにする
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// 赤い円を描画する
			DrawCircle(mouseX, mouseY, 20, GetColor(255, 0, 0), true);

			// 設定を元に戻す
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			SetWriteZBuffer3D(true);

			SetRenderTargetToShader(DX_SCREEN_BACK, false);

			SetDrawArea(0, 0, 0, 0);
		}
	}
}

void SceneMain::Draw()
{
	
	
	DrawGrid();
	m_pPlayer->Draw();
	m_pCamera->Draw();

	SetUseZBuffer3D(false);

	// 画面左上に黒のテクスチャを描画
	DrawGraph(0, 0, m_inkTexture, false);

	SetUseZBuffer3D(true);
	
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
	
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