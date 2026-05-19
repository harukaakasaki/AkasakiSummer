#pragma once
#include <DxLib.h>

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

private:
	int m_frameCount;
	float m_timeScale;
	Player* m_pPlayer;
	Camera* m_pCamera;
};

