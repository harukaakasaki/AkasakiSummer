#pragma once
#include "Scene.h"

class TitleScene : public Scene
{
public:
	TitleScene();
	virtual~TitleScene();
	void Init() override;
	void Update() override;
	void Draw() override;

	bool IsEnd() const override { return m_isEnd; }
	Scene* GetNextScene() override;

private:
	// モデル一覧
	int m_skyModelHandle;   // 空のモデル
	int m_fontHandle;       // フォントのハンドル
	int m_titleLogoHandle;  // タイトルロゴのハンドル

	// BGM関連
	int m_bgmHandle;
	int m_selectSeHandle;

	// 空の回転角
	float m_skyAngle = 0.0f;
	// 点滅アングル
	float m_blinkAngle = 0.0f;

	// シーンが終わったか
	bool m_isEnd = false;
};