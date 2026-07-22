#include "SceneMain.h"
#include "ResultScene.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>
#include "../GameObjects/Player.h"
#include "../Systems/Camera.h"
#include "../Systems/Pad.h"
#include "../Systems/CollisionManager.h"
#include "../Stages/StageManager.h"

namespace
{
	constexpr int kPlayerOrange = 1;// プレイヤーオレンジ
	constexpr int kPlayerBlue = 2;  // プレイヤーブルー
	constexpr int kTimer = 60*60;   // タイマーの時間

	// ステージの範囲
	constexpr float kStageMinX = -5900.0f;
	constexpr float kStageMaxX = 5820.0f;
	constexpr float kStageMinZ = -2120.0f;
	constexpr float kStageMaxZ = 1680.0f;
}

SceneMain::SceneMain() :
	m_frameCount(0),
	m_timeScale(1.0),
	m_gameUI(-1),
	m_reticleUI(-1),
	m_timer(kTimer),
	m_bgmHandle(-1)
{
	m_pStageManager = std::make_unique<StageManager>();
	m_pPlayer1 = std::make_unique<Player>(m_pStageManager.get(), DX_INPUT_PAD1,kPlayerOrange);
	m_pPlayer2 = std::make_unique<Player>(m_pStageManager.get(), DX_INPUT_PAD2,kPlayerBlue);
	m_pCamera1 = std::make_unique<Camera>();
	m_pCamera2 = std::make_unique<Camera>();
}

SceneMain::~SceneMain()
{
	StopSoundMem(m_bgmHandle);
	DeleteSoundMem(m_bgmHandle);
}


void SceneMain::Init()
{
	// カリングの設定（裏面のポリゴンは見えないようにする）
	SetUseBackCulling(true);

	// Zバッファの設定
	SetUseZBuffer3D(true);	    // Zバッファを使う
	SetWriteZBuffer3D(true);	// 描画する物体はZバッファにも距離を書き込む

	// 背景の色設定
	SetBackgroundColor(250, 250, 250);

	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 300.0f, -700), VGet(0.0f, 0.0f, 0.0f));
	SetupCamera_Perspective(DX_PI_F / 3.0f);


	m_pStageManager->Init();

	// カメラの更新
	m_pCamera1->Init(DX_INPUT_PAD1);
	m_pCamera2->Init(DX_INPUT_PAD2);
	m_pPlayer1->Init();
	m_pPlayer2->Init();

	// プレイヤー1の初期化
	float p1Angle = 0.0f;
	m_pPlayer1->SetPos(VGet(5000.0f, 0.0f, 0.0f));
	m_pPlayer1->SetAngleY(p1Angle);
	m_pCamera1->SetYaw(p1Angle);

	// プレイヤー2の初期化
	float p2Angle = DX_PI_F;
	m_pPlayer2->SetPos(VGet(-5000.0f, 0.0f, 0.0f));
	m_pPlayer2->SetAngleY(p2Angle);
	m_pCamera2->SetYaw(p2Angle);
	
	// 各プレイヤーのカメラの位置設定
	m_pCamera1->Update(m_pPlayer1->GetPos());
	m_pCamera2->Update(m_pPlayer2->GetPos());

	m_gameUI = LoadGraph("data/UI/GameUI_1.png");
	assert(m_gameUI != -1);
	m_reticleUI = LoadGraph("data/UI/reticle.png");
	assert(m_reticleUI != -1);

	// BGM
	m_bgmHandle = LoadSoundMem("data/bgm/game_rock_bgm.mp3");
	assert(m_bgmHandle != -1);
	ChangeVolumeSoundMem(200, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

	// コリジョンマネージャーの生成
	m_pCollisionManager = std::make_unique<CollisionManager>();

	m_pPlayerList.clear();
	m_pPlayerList.push_back(m_pPlayer1.get());
	m_pPlayerList.push_back(m_pPlayer2.get());

}

void SceneMain::Update()
{
	Pad::Update();
	m_frameCount++;

	if (m_gameState == GameState::Playing)
	{
		m_pPlayer1->Update(m_pCamera1->GetYaw(), m_pCamera1->GetPitch(), m_timeScale);
		m_pPlayer2->Update(m_pCamera2->GetYaw(), m_pCamera2->GetPitch(), m_timeScale);

		m_pCamera1->Update(m_pPlayer1->GetPos());
		m_pCamera2->Update(m_pPlayer2->GetPos());

		auto& bullet1 = m_pPlayer1->GetBullets();
		m_pCollisionManager->Update(m_pPlayerList, bullet1);
		auto& bullet2 = m_pPlayer2->GetBullets();
		m_pCollisionManager->Update(m_pPlayerList, bullet2);

		m_pStageManager->Update();
		InkPaint();

		VECTOR player1Pos = m_pPlayer1->GetPos();
		VECTOR player2Pos = m_pPlayer2->GetPos();

		// ステージ幅
		if (player1Pos.x < kStageMinX)player1Pos.x = kStageMinX;
		if (player2Pos.x < kStageMinX)player2Pos.x = kStageMinX;

		if (player1Pos.x > kStageMaxX)player1Pos.x = kStageMaxX;
		if (player2Pos.x > kStageMaxX)player2Pos.x = kStageMaxX;

		if (player1Pos.z < kStageMinZ)player1Pos.z = kStageMinZ;
		if (player2Pos.z < kStageMinZ)player2Pos.z = kStageMinZ;

		if (player1Pos.z > kStageMaxZ)player1Pos.z = kStageMaxZ;
		if (player2Pos.z > kStageMaxZ)player2Pos.z = kStageMaxZ;

		m_pPlayer1->SetPos(player1Pos);
		m_pPlayer2->SetPos(player2Pos);

		// ゲームタイマーを減らす
		m_timer--;

		if (m_timer <= 0)
		{
			m_timer = 0;
			m_gameState = GameState::Result;

			// オレンジとブルーの塗り割合を取得する
			int orange = m_pStageManager->GetPaintPercent(kPlayerOrange);
			int blue = m_pStageManager->GetPaintPercent(kPlayerBlue);

			// 勝敗の判定
			if (orange > blue)
			{
				// オレンジの勝利
				printfDx("オレンジの勝利!");
			}
			else if (blue > orange)
			{
				// ブルーの勝利
				printfDx("ブルーの勝利!");
			}
			else
			{
				// 引き分け
				printfDx("引き分け!");
			}
			// ゲーム終了フラグ
			m_isFinish = true;
		}
	}
}

// マウスでインクを塗る処理
void SceneMain::InkPaint()
{
	//int mouseX, mouseY;
	//GetMousePoint(&mouseX, &mouseY);
	//// マウスの座標を3Dに変換する
	//VECTOR rayStart = ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 0.0f));
	//VECTOR rayEnd   = ConvScreenPosToWorldPos(VGet((float)mouseX, (float)mouseY, 1.0f));
	//float vecY = rayEnd.y - rayStart.y;
	//float paintX = 0.0f;
	//float paintZ = 0.0f;
	//if (std::abs(vecY) > 0.0001f)
	//{
	//	float t = -rayStart.y / vecY;
	//	paintX = rayStart.x + t * (rayEnd.x - rayStart.x);
	//	paintZ = rayStart.z + t * (rayEnd.z - rayStart.z);
	//}
	//// もしマウスの左クリックを押したら、テクスチャにオレンジが付く
	//if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	//{
	//	m_pStageManager->Paint(paintX, paintZ, 1,100.0f);
	//}
	//// もしマウスの右クリックを押したら、テクスチャにブルーが付く
	//else if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	//{
	//	m_pStageManager->Paint(paintX, paintZ, 2,100.0f);
	//}
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

	// レティクル
	DrawGraph(190, 150, m_reticleUI, true);
	
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

	// レティクル
	DrawGraph(830, 150, m_reticleUI, true);

	// 描画範囲を元に戻す
	SetDrawArea(0, 0, 1280, 720);
	SetCameraScreenCenter(640.0f, 360.0f);
	/*SetUseZBuffer3D(false);

	SetUseZBuffer3D(true);*/
	
#ifdef _DEBUG
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
#endif // DEBUG
	// 画面の中央に線を引く
	DrawLine(640, 0, 640, 720, GetColor(0, 0, 0));

	// 各プレイヤーの塗り割合を取得
	float orangePercent = m_pStageManager->GetPaintPercent(kPlayerOrange);
	float bluePercent = m_pStageManager->GetPaintPercent(kPlayerBlue);

	// UIの描画
	int width, height;
	GetGraphSize(m_gameUI, &width, &height);

	DrawExtendGraph(270, 10, 100 + width / 4, 100 + height / 5, m_gameUI, true);

	int seconds = m_timer / 60;
	DrawFormatString(625, 40, GetColor(255, 255, 255), "%d", seconds);

	// 割合を描画
	DrawFormatString(560, 120, GetColor(255, 255, 255), "%.2f%%", orangePercent);
	DrawFormatString(680, 120, GetColor(255, 255, 255), "%.2f%%", bluePercent);
}

bool SceneMain::IsEnd() const
{
	return m_isFinish;
}

Scene* SceneMain::GetNextScene()
{
	return new ResultScene();// 次のシーンへ移行
}

void SceneMain::DrawGrid()
{
	//// 直線の始点と終点
	//VECTOR startPos;
	//VECTOR endPos;

	//for (int z = -300; z <= 300; z += 100)
	//{
	//	startPos = VGet(-300.0f, 0.0f, static_cast<float>(z));
	//	endPos = VGet(300.0f, 0.0f, static_cast<float>(z));
	//	DrawLine3D(startPos, endPos, 0x0000ff);
	//}
	//for (int x = -300; x <= 300; x += 100)
	//{
	//	startPos = VGet(static_cast<float>(x), 0.0f, -300.0f);
	//	endPos = VGet(static_cast<float>(x), 0.0f, 300.0f);
	//	DrawLine3D(startPos, endPos, 0x0000ff);
	//}
}