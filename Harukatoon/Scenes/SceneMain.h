#pragma once
#include <DxLib.h>
#include <vector>

// プロトタイプ宣言
class Player;
class Camera;
class StageManager;

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void Update();
	void Draw();

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
	float m_timeScale;
	Player* m_pPlayer1;
	Player* m_pPlayer2;
	Camera* m_pCamera1;
	Camera* m_pCamera2;
	StageManager* m_pStageManager;

	int m_gameUI;
};

