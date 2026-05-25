#pragma once
#include <DxLib.h>
#include <vector>

// プロトタイプ宣言
class Player;
class Camera;

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
	int m_inkTexture;
	int m_frameCount;
	float m_timeScale;
	Player* m_pPlayer;
	Camera* m_pCamera;
};

