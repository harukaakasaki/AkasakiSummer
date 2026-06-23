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

private:
	int m_frameCount;
	float m_timeScale;
	Player* m_pPlayer1;
	Player* m_pPlayer2;
	Camera* m_pCamera;
	StageManager* m_pStageManager;

	int m_gameUI;
};

