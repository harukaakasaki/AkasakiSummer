#include "TitleScene.h"
#include "../Systems/Pad.h"
#include "../Systems/Game.h"
#include "SceneMain.h"
#include <DxLib.h>

namespace
{
	
}

TitleScene::TitleScene() :
	m_skyModelHandle(-1),
	m_titleLogoHandle(-1),
	m_skyAngle(0.0f),
	m_isEnd(false),
	m_bgmHandle(0),
	m_selectSeHandle(0)
{
}

TitleScene::~TitleScene()
{
	MV1DeleteModel(m_skyModelHandle);
	DeleteGraph(m_titleLogoHandle);

	StopSoundMem(m_bgmHandle);
	DeleteSoundMem(m_bgmHandle);
	StopSoundMem(m_selectSeHandle);
	DeleteSoundMem(m_selectSeHandle);
}

void TitleScene::Init()
{
	SetBackgroundColor(0, 0, 0);

	// モデルを読み込む
	m_skyModelHandle = MV1LoadModel("data/Sky_Night02.mv1");
	m_titleLogoHandle = LoadGraph("data/UI/Harukatoon_Logo.png");

	// BGM
	m_bgmHandle = LoadSoundMem("data/bgm/title_bgm.mp3");
	m_selectSeHandle = LoadSoundMem("data/bgm/select_se.mp3");

	ChangeVolumeSoundMem(180, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);


	// フォントを作る
	m_fontHandle = CreateFontToHandle("Nikkyou Sans", 70, -1, DX_FONTTYPE_NORMAL);

	MV1SetPosition(m_skyModelHandle, VGet(500, -600, 0));

	// カメラ位置
	SetCameraPositionAndTarget_UpVecY(
		VGet(500, 300, -1000),// カメラ位置
		VGet(500, 300, -200));// 注視点

	SetupCamera_Perspective(DX_PI_F / 3.0f);
	SetCameraNearFar(100.0f, 15000.0f);
}

void TitleScene::Update()
{
	Pad::Update();

	// 待機アニメーション
	m_animation.Play(m_playerIdleAnim, true, 1.0f);
	m_animation.Play(m_enemyIdleAnim, true, 1.0f);
	// アニメーションの再生速度
	m_animation.SetGlobalSpeed(0.7f);
	m_animation.Update();

	// 空の回転
	m_skyAngle += 0.002f;
	// 点滅
	m_blinkAngle += 0.08f;

	if (Pad::IsTrigger(DX_INPUT_PAD1,PAD_INPUT_1))
	{
		// SEの再生
		ChangeVolumeSoundMem(200, m_selectSeHandle);
		PlaySoundMem(m_selectSeHandle, DX_PLAYTYPE_BACK);
		m_isEnd = true;
	}
}

void TitleScene::Draw()
{
	// スカイドームの回転
	MV1SetRotationXYZ(m_skyModelHandle, VGet(0, m_skyAngle, 0));

	// モデルを描画
	MV1DrawModel(m_skyModelHandle);
	/*MV1DrawModel(m_playerModelHandle);
	MV1DrawModel(m_enemyModelHandle);*/

	// 2D描画の設定
	SetUseZBufferFlag(false);
	// タイトル画面を描画
//	DrawGraph(0, 0, m_titleLogoHandle, TRUE);
	DrawString(0, 50, "タイトルシーン", GetColor(255, 255, 255));
	

	SetUseZBufferFlag(true);

	DrawStringToHandle(450, 300, "ハルカトゥーン", GetColor(255, 255, 255), m_fontHandle);

	// スタートボタンを点滅させる
	int alpha = static_cast<int>((sinf(m_blinkAngle) * 0.5f + 0.5f) * 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	
	DrawStringToHandle(460, 505, "Aボタンでスタート！", GetColor(0, 0, 0), m_fontHandle);
	DrawStringToHandle(450, 500, "Aボタンでスタート！", GetColor(255, 255, 255), m_fontHandle);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

Scene* TitleScene::GetNextScene()
{
	return new SceneMain();// 次のシーンへ移行
}
