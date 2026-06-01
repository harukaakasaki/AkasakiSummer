#pragma once

// ステージを管理してもらうクラス
#include <vector>
class StageManager
{
public:

	StageManager();
	~StageManager();

	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// 塗った所をその色に書き換える
	/// </summary>
	/// <param name="x">塗った座標X</param>
	/// <param name="y">塗った座標Y</param>
	/// <param name="who">誰が塗ったか</param>
	void Paint(float x,float y,float who);

private:

	// 2Dマップデータ
	std::vector<std::vector<int>> m_2dMap;
	// 1マスのサイズ
	int m_pixelSize;
	// ステージ縦横のマス数
	int m_MapWidthSize;
	int m_MapHeightSize;
	// インクのテクスチャ
	int m_pinkTextureHandle;
	int m_greenTextureHandle;
	// インクのノーマルマップ
	int m_nPinkTextureHandle;
	int m_nGreenTextureHandle;
	// インクのシェーダーハンドル
	int m_InkShaderHandle;

};

