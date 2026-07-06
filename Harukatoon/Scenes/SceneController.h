#pragma once
#include "Scene.h"

class SceneController
{
public:
	void Init();
	void Update();
	void Draw();

private:
	Scene* m_pScene = nullptr;// 現在のシーン

	// フェードイン・フェードアウトの制御
	bool m_isFadeIn = false;
	bool m_isFadeOut = true;
	int m_fadeAlpha = 255;// フェードのアルファ値
	Scene* m_pNextScene = nullptr;// 次のシーン


};


