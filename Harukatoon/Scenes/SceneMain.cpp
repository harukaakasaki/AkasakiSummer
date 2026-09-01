#include "SceneMain.h"
#include "ResultScene.h"
#include <DxLib.h>
#include <cmath>
#include <algorithm>
#include <cassert>

#include "../GameObjects/Player.h"
#include "../Systems/Camera.h"
#include "../Systems/Game.h"
#include "../Systems/Pad.h"
#include "../Systems/CollisionManager.h"
#include "../Stages/StageManager.h"

namespace
{ 
	constexpr int kPlayerOrange = 1;          // プレイヤーオレンジ
	constexpr int kPlayerBlue = 2;            // プレイヤーブルー
	constexpr int kTimer = 10*60;             // タイマーの時間
	constexpr int kBGMVol = 100;              // ゲームシーンのBGMの大きさ
	constexpr float kPlayer1FirstPos = 5000;  // プレイヤー1の初期位置
	constexpr float kPlayer2FirstPos = -5000; // プレイヤー2の初期位置

	// ステージの範囲
	constexpr float kStageMinX = -5900.0f;
	constexpr float kStageMaxX = 5820.0f;
	constexpr float kStageMinZ = -2120.0f;
	constexpr float kStageMaxZ = 2080.0f;

	// BGM一覧
	const std::vector<std::string>kBgmPathList =
	{
		"data/bgm/game_bgm_1.mp3",
		"data/bgm/game_bgm_2.mp3",
		"data/bgm/game_bgm_3.mp3"
	};
}

SceneMain::SceneMain() :
	m_frameCount(0),
	m_timeScale(1.0),
	m_gameUI(-1),
	m_reticleUI(-1),
	m_player1DamageUI(-1),
	m_player2DamageUI(-1),
	m_timer(kTimer),
	m_bgmHandle(-1),
	m_fontHandle(-1)
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

	m_pStageManager->Init();

	// カメラの更新
	m_pCamera1->Init(DX_INPUT_PAD1);
	m_pCamera2->Init(DX_INPUT_PAD2);
	m_pPlayer1->Init();
	m_pPlayer2->Init();

	// プレイヤー1の初期化
	float p1Angle = 0.0f;
	m_pPlayer1->SetRespawnPos(VGet(kPlayer1FirstPos, 0.0f, 0.0f));
	m_pPlayer1->SetAngleY(p1Angle);
	m_pCamera1->SetYaw(p1Angle);

	// プレイヤー2の初期化
	float p2Angle = DX_PI_F;
	m_pPlayer2->SetRespawnPos(VGet(kPlayer2FirstPos, 0.0f, 0.0f));
	m_pPlayer2->SetAngleY(p2Angle);
	m_pCamera2->SetYaw(p2Angle);
	
	// 各プレイヤーのカメラの位置設定
	m_pCamera1->Update(m_pPlayer1->GetPos());
	m_pCamera2->Update(m_pPlayer2->GetPos());

	m_gameUI = LoadGraph("data/UI/GameUI_1.png");
	assert(m_gameUI != -1);
	m_reticleUI = LoadGraph("data/UI/reticle.png");
	assert(m_reticleUI != -1);
	m_player1DamageUI = LoadGraph("data/UI/player1DamageUI.png");
	assert(m_player1DamageUI != -1);
	m_player2DamageUI = LoadGraph("data/UI/player2DamageUI.png");
	assert(m_player2DamageUI != -1);

	// BGM
	int bgmIndex = GetRand(static_cast<int>(kBgmPathList.size()) - 1);
	m_bgmHandle = LoadSoundMem(kBgmPathList[bgmIndex].c_str());
	assert(m_bgmHandle != -1);
	ChangeVolumeSoundMem(kBGMVol, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

	// フォントを作る
	m_fontHandle = CreateFontToHandle("Nikkyou Sans", 40, -1, DX_FONTTYPE_NORMAL);

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

	// ゲーム内の処理
	if (m_gameState == GameState::Playing)
	{
		m_pPlayer1->Update(m_pCamera1->GetYaw(), m_pCamera1->GetPitch(), m_timeScale);
		m_pPlayer2->Update(m_pCamera2->GetYaw(), m_pCamera2->GetPitch(), m_timeScale);

		int stageHandle = m_pStageManager->GetStageModelHandle();
		m_pCamera1->Update(m_pPlayer1->GetPos(),stageHandle);
		m_pCamera2->Update(m_pPlayer2->GetPos(),stageHandle);

		auto& bullet1 = m_pPlayer1->GetBullets();
		m_pCollisionManager->Update(m_pPlayerList, bullet1);
		auto& bullet2 = m_pPlayer2->GetBullets();
		m_pCollisionManager->Update(m_pPlayerList, bullet2);

		m_pStageManager->Update();

		// プレイヤーを範囲内に収める
		VECTOR player1Pos = m_pPlayer1->GetPos();
		player1Pos.x = std::clamp(player1Pos.x, kStageMinX, kStageMaxX);
		player1Pos.z = std::clamp(player1Pos.z, kStageMinZ, kStageMaxZ);
		m_pPlayer1->SetPos(player1Pos);

		VECTOR player2Pos = m_pPlayer2->GetPos();
		player2Pos.x = std::clamp(player2Pos.x, kStageMinX, kStageMaxX);
		player2Pos.z = std::clamp(player2Pos.z, kStageMinZ, kStageMaxZ);
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
				m_winnerState = WinnerType::Orange;
			}
			else if (blue > orange)
			{
				// ブルーの勝利
				m_winnerState = WinnerType::Blue;
			}
			else
			{
				// 引き分け
				m_winnerState = WinnerType::None;
			}
			// ゲーム終了フラグ
			m_isFinish = true;
		}
	}
}

void SceneMain::Draw()
{
	// プレイヤー1は描画範囲を左半分にする
	SetDrawArea(0, 0, Game::kSplitWidth, Game::kScreenHeight);
	//3Dカメラの描画範囲を左半分に合わせる
	SetCameraScreenCenter(Game::kCamera1CenterX, Game::kCameraCenterY);

	// プレイヤー1のカメラを描画
	m_pCamera1->Draw();
	 
	// プレイヤー1に映る世界の描画
	DrawGrid();
	m_pPlayer1->Draw();
	m_pPlayer2->Draw();
	m_pStageManager->Draw();

	// レティクル
	DrawRotaGraph(static_cast<int>(Game::kCamera1CenterX), static_cast<int>(Game::kScreenCenterY-120),1.0,0.0, m_reticleUI, true);

	// プレイヤー1のダメージUIの描画処理
	m_pPlayer1->DrawDamageUI(0);
	
	// プレイヤー2は描画範囲を右半分にする
	SetDrawArea(Game::kSplitWidth, 0, Game::kScreenWidth, Game::kScreenHeight);
	//3Dカメラの描画範囲を右半分に合わせる
	SetCameraScreenCenter(Game::kCamera2CenterX, Game::kCameraCenterY);
	
	// プレイヤー2のカメラを描画
	m_pCamera2->Draw();

	// プレイヤー2に映る世界の描画
	DrawGrid();
	m_pPlayer1->Draw();
	m_pPlayer2->Draw();
	m_pStageManager->Draw();

	// レティクル
	DrawRotaGraph(static_cast<int>(Game::kCamera2CenterX), static_cast<int>(Game::kScreenCenterY-120), 1.0, 0.0, m_reticleUI, true);

	// プレイヤー2のダメージUIの描画処理
	m_pPlayer2->DrawDamageUI(Game::kSplitWidth);

	// 描画範囲を元に戻す
	SetDrawArea(0, 0, Game::kScreenWidth, Game::kScreenHeight);
	SetCameraScreenCenter(static_cast<float>(Game::kScreenCenterX), static_cast<float>(Game::kScreenCenterY));
	
#ifdef _DEBUG
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
#endif // DEBUG
	// 画面の中央に線を引く
	DrawLine(Game::kScreenCenterX, 0, Game::kScreenCenterX, Game::kScreenHeight, GetColor(0, 0, 0));

	// 各プレイヤーの塗り割合を取得
	float orangePercent = m_pStageManager->GetPaintPercent(kPlayerOrange);
	float bluePercent = m_pStageManager->GetPaintPercent(kPlayerBlue);

	// UIの描画
	DrawRotaGraph(Game::kScreenCenterX, Game::kScreenCenterY, 0.9, 0, m_gameUI, TRUE);

	int seconds = m_timer / 60;
//	DrawFormatString(Game::kScreenCenterX - 10, 60, GetColor(255, 255, 255), "%d", seconds);
	DrawFormatStringToHandle(Game::kScreenCenterX -30, 85, GetColor(255, 255, 255), m_fontHandle, "%d", seconds);

	// 各インクの割合を描画
	DrawFormatStringToHandle(Game::kScreenCenterX - 140, 211, GetColor(0, 0, 0), m_fontHandle, "%d%%", static_cast<int>(orangePercent));
	DrawFormatStringToHandle(Game::kScreenCenterX - 145, 210, GetColor(255, 255, 255), m_fontHandle, "%d%%", static_cast<int>(orangePercent));
	DrawFormatStringToHandle(Game::kScreenCenterX + 100, 211, GetColor(0, 0, 0), m_fontHandle, "%d%%", static_cast<int>(bluePercent));
	DrawFormatStringToHandle(Game::kScreenCenterX + 95, 210, GetColor(255, 255, 255), m_fontHandle, "%d%%", static_cast<int>(bluePercent));
}

bool SceneMain::IsEnd() const
{
	return m_isFinish;
}

Scene* SceneMain::GetNextScene()
{
	return new ResultScene(m_winnerState);// 次のシーンへ移行
}

void SceneMain::DrawGrid()
{
#ifdef _DEBUG
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
#endif // _DEBUG
}