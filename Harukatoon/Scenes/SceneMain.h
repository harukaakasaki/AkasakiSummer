#pragma once
#include <DxLib.h>
#include <vector>
#include "Scene.h"

// プロトタイプ宣言
class Player;
class Camera;
class StageManager;

class SceneMain :public Scene
{
public:
	SceneMain();
	~SceneMain();

	void Init()override;
	void Update()override;
	void Draw()override;

	bool IsEnd()const override;
	Scene* GetNextScene() override;

private:
	void DrawGrid();
	void InkPaint();// 色を塗る

	// ゲームの状態
	enum class GameState
	{
		Playing,// ゲーム中
		Result  // 結果表示
	};

	GameState m_gameState = GameState::Playing;
	int m_timer;

private:
	int m_frameCount;
	int m_bgmHandle;
	float m_timeScale;
	bool m_isFinish = false;
	Player* m_pPlayer1;
	Player* m_pPlayer2;
	Camera* m_pCamera1;
	Camera* m_pCamera2;
	StageManager* m_pStageManager;

	int m_gameUI;
};

