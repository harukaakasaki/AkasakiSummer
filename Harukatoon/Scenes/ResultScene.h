#pragma once
#include "Scene.h"
#include "../Systems/Animation.h"



class ResultScene : public Scene
{
public:
	ResultScene();
	~ResultScene();
	void Init() override;
	void Update() override;
	void Draw() override;

	bool IsEnd()const override { return m_isEnd; }
	Scene* GetNextScene() override;
private:
	bool m_isEnd = false;
	// モデル一覧
	int m_playerModelHandle;// プレイヤーのモデル
	int m_fontHandle;       // フォントのハンドル
	int m_resultUIHandle;   // リザルトのUIハンドル

	// BGM関連
	int m_bgmHandle;
	int m_selectSeHandle;

	// アニメーション
	Animation m_animation;
	int m_playerIdleAnim;

	// 空の回転角
	float m_skyAngle = 0.0f;
	// プレイヤーの回転角
	float m_playerAngle = 0.0f;
	// 点滅アングル
	float m_blinkAngle = 0.0f;
};

