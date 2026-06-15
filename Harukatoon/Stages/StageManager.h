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
	void Paint(float x,float y,float who, float paintRadius);

private:

	// 2Dマップデータ
	std::vector<std::vector<int>> m_2dMap;
	// 1マスのサイズ
	int m_cellSize;
	// ステージ縦横のマス数
	int m_mapWidthSize;
	int m_mapHeightSize;
	// インクのテクスチャ
	int m_orangeTextureHandle;
	int m_blueTextureHandle;
	// インクのノーマルマップ
	int m_nOrangeTextureHandle;
	int m_nBlueTextureHandle;
	// インクのシェーダーハンドル
	int m_inkShaderHandle;
	// 塗れる範囲のハンドル
	int m_inkCanvasHandle;
	// 塗れる範囲のノーマルハンドル
	int m_inkNormalCanvasHandle;

};

