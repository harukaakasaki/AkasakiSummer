#include "SceneController.h"
#include "TitleScene.h"
#include "../Systems/Game.h"
#include <DxLib.h>
#include "Scene.h"

namespace
{
	constexpr int kFadeSpeed = 10;// フェードの速度
	constexpr float kFadeOutTime = 360.0f;// フェードアウトの時間
}

void SceneController::Init()
{
	m_pScene = new TitleScene();
	m_pScene->Init();
}

void SceneController::Update()
{
	// フェードイン
	if (!m_isFadeIn)
	{
		// 現在のシーンを更新
		m_pScene->Update();

		if (m_pScene->IsEnd())
		{
			m_pNextScene = m_pScene->GetNextScene();

			m_isFadeIn = true;
			m_isFadeOut = true;
			m_fadeAlpha = 0;
		}
	}
	else
	{
		// フェードアウト
		if (m_isFadeOut)
		{
			m_fadeAlpha += kFadeSpeed;
			if (m_fadeAlpha >= kFadeOutTime)
			{
				m_fadeAlpha = kFadeOutTime;
				// シーン切り替え
				delete m_pScene;
				m_pScene = m_pNextScene;
				m_pScene->Init();

				// フェードイン開始
				m_isFadeOut = false;
			}
		}
		else
		{
			// フェードイン
			m_fadeAlpha -= kFadeSpeed;
			if (m_fadeAlpha <= 0)
			{
				m_fadeAlpha = 0;
				m_isFadeIn = false;
			}
		}
	}
}

void SceneController::Draw()
{

	m_pScene->Draw();
	if (m_isFadeIn)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
