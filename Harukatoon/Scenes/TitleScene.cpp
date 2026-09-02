#include "TitleScene.h"
#include "../Systems/Pad.h"
#include "../Systems/Game.h"
#include "SceneMain.h"
#include <DxLib.h>

namespace
{
	constexpr int kBGMVol = 200;                             // ゲームシーンのBGMの大きさ
}

TitleScene::TitleScene() :
	m_titleLogoHandle(-1),
	m_fontHandle(-1),
	m_isEnd(false),
	m_bgmHandle(0),
	m_aButtonUIHandle(-1),
	m_pressInkUIHandle(-1)
{
}

TitleScene::~TitleScene()
{
	DeleteGraph(m_titleLogoHandle);

	StopSoundMem(m_bgmHandle);
	DeleteSoundMem(m_bgmHandle);
}

void TitleScene::Init()
{
	// モデルを読み込む
	m_titleLogoHandle = LoadGraph("data/UI/Harukatoon_Title.png");
	m_pressInkUIHandle = LoadGraph("data/UI/PressInk.png");
	m_aButtonUIHandle = LoadGraph("data/UI/aButton.png");

	// BGM
	m_bgmHandle = LoadSoundMem("data/bgm/title_bgm.mp3");

	ChangeVolumeSoundMem(kBGMVol, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

	// フォントを作る
	m_fontHandle = CreateFontToHandle("Nikkyou Sans", 70, -1, DX_FONTTYPE_NORMAL);

	// カメラ位置
	SetCameraPositionAndTarget_UpVecY(
		VGet(500, 300, -1000),// カメラ位置
		VGet(500, 300, -200));// 注視点

	SetupCamera_Perspective(DX_PI_F / 3.0f);
	SetCameraNearFar(100.0f, 15000.0f);
}

void TitleScene::Update()
{
	// パッドクラスの更新
	Pad::Update();

	// 点滅
	m_blinkAngle += 0.08f;

	if (Pad::IsTrigger(DX_INPUT_PAD1, PAD_INPUT_1))
	{
		m_isEnd = true;
	}
}

void TitleScene::Draw()
{
	// 2D描画の設定
	SetUseZBufferFlag(false);
	// タイトル画面を描画
	DrawRotaGraph(Game::kScreenWidth/2, Game::kScreenHeight/2,1.0,0, m_titleLogoHandle, TRUE);
	DrawRotaGraph(Game::kScreenWidth/2, Game::kScreenHeight/2,1.0,0, m_pressInkUIHandle, TRUE);
	

	SetUseZBufferFlag(true);

	// スタートボタンを点滅させる
	int alpha = static_cast<int>((sinf(m_blinkAngle) * 0.5f + 0.5f) * 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.0, 0, m_aButtonUIHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

Scene* TitleScene::GetNextScene()
{
	return new SceneMain();// 次のシーンへ移行
}