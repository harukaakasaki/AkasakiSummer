#include "ResultScene.h"
#include "../Systems/Pad.h"
#include "../Systems/Game.h"
#include "TitleScene.h"
#include <DxLib.h>
#include <cmath>
#include "../Stages/StageManager.h"

namespace
{
	// アニメーションのインデックス
	const char* const kPlayerAnim = "CharacterArmature|Idle";// プレイヤー待機アニメーション

	constexpr int kBGMVol = 150;                             // ゲームシーンのBGMの大きさ
}

ResultScene::ResultScene(WinnerType winner) :
	m_winState(winner),
	m_isEnd(false),
	m_fontHandle(-1),
	m_playerModelHandle(-1),
	m_playerIdleAnim(0),
	m_resultUIHandle(-1),
	m_winUIHandle(-1),
	m_aButtonUIHandle(-1),
	m_bgmHandle(0),
	m_selectSeHandle(0)
{
}

ResultScene::~ResultScene()
{
	MV1DeleteModel(m_playerModelHandle);

	StopSoundMem(m_bgmHandle);
	DeleteSoundMem(m_bgmHandle);
	StopSoundMem(m_selectSeHandle);
	DeleteSoundMem(m_selectSeHandle);
}

void ResultScene::Init()
{
	SetBackgroundColor(0, 0, 0);

	// モデルを読み込む
	if (m_winState == WinnerType::Orange)
	{
		m_playerModelHandle = MV1LoadModel("data/Models/playerOrange.mv1");
	}
	else if (m_winState == WinnerType::Blue)
	{
		m_playerModelHandle = MV1LoadModel("data/Models/playerBlue.mv1");
	}
	else
	{
		m_playerModelHandle = MV1LoadModel("data/Models/playerOrange.mv1");
	}
	
	m_resultUIHandle = LoadGraph("data/UI/resultUI.png");
	m_winUIHandle = LoadGraph("data/UI/resultWin.png");
	m_aButtonUIHandle = LoadGraph("data/UI/resultA.png");

	// BGM
	m_bgmHandle = LoadSoundMem("data/BGM/result_bgm.mp3");
	m_selectSeHandle = LoadSoundMem("data/bgm/select_se.mp3");

	ChangeVolumeSoundMem(100, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

	// フォントを作る
	m_fontHandle = CreateFontToHandle("Nikkyou Sans", 70, -1, DX_FONTTYPE_NORMAL);
	// カメラ位置
	SetCameraPositionAndTarget_UpVecY(
		VGet(500, 250, -400),// カメラ位置
		VGet(500, 250, -200));// 注視点

	// プレイヤーモデルの位置
	MV1SetPosition(m_playerModelHandle, VGet(500, 200, -200));
}

void ResultScene::Update()
{
	// パッドクラスの更新
	Pad::Update();

	// Aボタンが押されたらシーンを終了する
	if (Pad::IsTrigger(DX_INPUT_PAD1, PAD_INPUT_1))
	{
		// SEの再生
		ChangeVolumeSoundMem(kBGMVol, m_selectSeHandle);
		PlaySoundMem(m_selectSeHandle, DX_PLAYTYPE_BACK);
		m_isEnd = true;
	}

	// 空の回転
	m_skyAngle += 0.008f;
	// プレイヤーの回転
	m_playerAngle += 0.008f;
	m_blinkAngle += 0.08f;
}

void ResultScene::Draw()
{
	// UIを描画
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 0.9, 0, m_resultUIHandle, TRUE);
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.0, 0, m_winUIHandle, TRUE);
	
	// モデルの表示
	MV1DrawModel(m_playerModelHandle);

	// プレイヤーのモデルの向きの調整
	MV1SetRotationXYZ(m_playerModelHandle, VGet(0, m_playerAngle, 0));

	// スタートボタンを点滅させる
	int alpha = static_cast<int>((sinf(m_blinkAngle) * 0.5f + 0.5f) * 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.0, 0, m_aButtonUIHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

// シーン切り替え
Scene* ResultScene::GetNextScene()
{
	return new TitleScene;
}
