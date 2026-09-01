#pragma once
#include "Scene.h"
#include "../Systems/Animation.h"

enum class WinnerType
{
	None,   // 引き分け（強制的にオレンジの勝利）
	Orange, // オレンジ勝利
	Blue    // ブルー勝利
};

class ResultScene : public Scene
{
	
public:
	explicit ResultScene(WinnerType winner);
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
	int m_winUIHandle;      // WinのUIハンドル
	int m_aButtonUIHandle;  // AのUIハンドル

	// BGM関連
	int m_bgmHandle;
	int m_selectSeHandle;

	// 勝者判定変数
	WinnerType m_winState;

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

