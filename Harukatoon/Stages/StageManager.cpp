#include "StageManager.h"
#include <DxLib.h>
#include <cassert>

namespace
{
}

StageManager::StageManager():
	m_MapWidthSize(0),
	m_MapHeightSize(0),
	m_pixelSize(0),
	m_pinkTextureHandle(-1),
	m_greenTextureHandle(-1),
	m_nPinkTextureHandle(-1),
	m_nGreenTextureHandle(-1),
	m_InkShaderHandle(-1)
{
}

StageManager::~StageManager()
{
}

void StageManager::Init()
{
	// ピンクのインクテクスチャ
	m_pinkTextureHandle = LoadGraph("data/Ink/ink_pink.png");
	assert(m_pinkTextureHandle != -1);
	// グリーンのインクテクスチャ
	m_greenTextureHandle = LoadGraph("data/Ink/ink_green.png");
	assert(m_greenTextureHandle != -1);
	// ピンクのノーマルマップ
	m_pinkTextureHandle = LoadGraph("data/Ink/ink_pink_n.png");
	assert(m_pinkTextureHandle != -1);
	// グリーンのノーマルマップ
	m_greenTextureHandle = LoadGraph("data/Ink/ink_green_n.png");
	assert(m_greenTextureHandle != -1);
	// インクのシェーダー
	m_InkShaderHandle = LoadPixelShader("InkShader.pso");
	assert(m_InkShaderHandle != -1);

	m_pixelSize = 200;
	m_MapWidthSize = 64;
	m_MapHeightSize = 48;

	m_2dMap.resize(m_MapHeightSize);// 縦48マスの空きを作る
	// メモリ内に48x64の紙を作成
	for (int i = 0; i < m_MapHeightSize; i++)
	{
		m_2dMap[i].resize(m_MapWidthSize,0);
	}
}
void StageManager::Update()
{
	// プレイヤーの弾が当たった座標に色を塗る
}
void StageManager::Draw()
{
	// 床のマスを描画
	for (int y = 0; y < m_MapHeightSize; y++)
	{
		for (int x = 0; x < m_MapWidthSize; x++)
		{
			if (m_2dMap[y][x] == 0)
			{
				int leftX = x * m_pixelSize;
				int topY = y * m_pixelSize;
				int rightX = (x + 1) * m_pixelSize;
				int bottomY = (y + 1) * m_pixelSize;

				//DrawBox(leftX, topY, rightX, bottomY, GetColor(0, 0, 0), TRUE);
			}
			
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUseAlphaTestFlag(TRUE);// アルファテストをONにする（アルファ値が0の部分は描画されないようになる）

	// インクの描画を開始（シェーダーON）
	if (m_InkShaderHandle != -1)
	{
		SetUsePixelShader(m_InkShaderHandle);
	}

	for (int y = 0; y < m_MapHeightSize; y++)
	{
		for (int x = 0; x < m_MapWidthSize; x++)
		{
			int leftX = x * m_pixelSize;
			int topY = y * m_pixelSize;
			int rightX = (x + 1) * m_pixelSize;
			int bottomY = (y + 1) * m_pixelSize;

			
			if (m_2dMap[y][x] == 1)// ピンクの場合
			{
				SetUseTextureToShader(0,m_pinkTextureHandle);
				SetUseTextureToShader(1,m_nPinkTextureHandle);
				DrawExtendGraph(leftX, topY, rightX, bottomY, m_pinkTextureHandle, TRUE);
			}
			else if (m_2dMap[y][x] == 2)// グリーンの場合
			{
				SetUseTextureToShader(0,m_greenTextureHandle);
				SetUseTextureToShader(1,m_nGreenTextureHandle);
				DrawExtendGraph(leftX, topY, rightX, bottomY, m_greenTextureHandle, TRUE);
			}
		}
	}

	if (m_InkShaderHandle != -1)
	{
		// インクを書き終えたら元の描画にリセットする
		SetUsePixelShader(-1);
	}

	SetUseAlphaTestFlag(FALSE);// アルファテストをOFFにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void StageManager::Paint(float x, float y, float who)
{
	int targetX = x / m_pixelSize;
	int targetY = y / m_pixelSize;

	if (targetX >= 0 && targetX < m_MapWidthSize && targetY >= 0 && targetY < m_MapHeightSize)
	{
		m_2dMap[targetY][targetX] = who;
	}
	
}

