#pragma once
#include "Scene.h"

class SceneController
{
public:
	void Init();
	void Update();
	void Draw();

private:
	// 現在のシーン
	Scene* m_pScene = nullptr;

	// フェードイン・フェードアウトの制御
	bool m_isFadeIn = false;
	bool m_isFadeOut = true;
	// フェードのアルファ値
	int m_fadeAlpha = 255;
	// 次のシーン
	Scene* m_pNextScene = nullptr;


};


