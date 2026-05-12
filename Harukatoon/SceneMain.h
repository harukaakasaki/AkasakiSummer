#pragma once
#include <DxLib.h>

// プロトタイプ宣言
class Player;

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
	Player* m_pPlayer;
};

