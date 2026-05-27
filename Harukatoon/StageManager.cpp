#include "StageManager.h"
#include <DxLib.h>

namespace
{
}

StageManager::StageManager():
	m_MapWidthSize(0),
	m_MapHeightSize(0),
	m_pixelSize(10)
{
}

StageManager::~StageManager()
{
}

void StageManager::Init()
{
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
	for (int y = 0; y < m_MapHeightSize; y++)
	{
		for (int x = 0; x < m_MapWidthSize; x++)
		{
			int leftX = x * m_pixelSize;
			int topY = y * m_pixelSize;
			int rightX = (x + 1) * m_pixelSize;
			int bottomY = (y + 1) * m_pixelSize;

			if (m_2dMap[y][x] == 0)
			{
				DrawBox(leftX, topY, rightX, bottomY, GetColor(190, 190, 190), TRUE);
			}
			else if (m_2dMap[y][x] == 1)// ピンクの場合
			{
				DrawBox(leftX, topY, rightX, bottomY, GetColor(255, 20, 140), TRUE);
			}
			else if (m_2dMap[y][x] == 2)// グリーンの場合
			{
				DrawBox(leftX, topY, rightX, bottomY, GetColor(0, 255, 0), TRUE);
			}
		}
	}
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

