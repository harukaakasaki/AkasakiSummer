#include "StageManager.h"
#include <DxLib.h>
#include <cassert>

namespace
{
}

StageManager::StageManager():
	m_MapWidthSize(0),
	m_MapHeightSize(0),
	m_cellSize(0),
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
	m_nPinkTextureHandle = LoadGraph("data/Ink/ink_pink_n.png");
	assert(m_nPinkTextureHandle != -1);
	// グリーンのノーマルマップ
	m_nGreenTextureHandle = LoadGraph("data/Ink/ink_green_n.png");
	assert(m_nGreenTextureHandle != -1);
	// インクのシェーダー
	m_InkShaderHandle = LoadPixelShader("InkShader.pso");
	assert(m_InkShaderHandle != -1);

	m_cellSize = 10.0f;
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUseAlphaTestFlag(TRUE);// アルファテストをONにする（アルファ値が0の部分は描画されないようになる）

	SetUseBackCulling(FALSE);// 裏面も描画するようにする（これをしないと裏返ったポリゴンが見えなくなる）

	// インクの描画を開始（シェーダーON）
	if (m_InkShaderHandle != -1)
	{
		SetUsePixelShader(m_InkShaderHandle);
	}

	// 3D空間にインクを描画する
	float size = m_cellSize;// インクの大きさ

	for (int z = 0; z < m_MapHeightSize; z++)
	{
		for (int x = 0; x < m_MapWidthSize; x++)
		{
			if (m_2dMap[z][x] == 0)
			{
				continue;
			}
			// 2Dのマス目を3D空間に変換して描画する
			float offsetX = (m_MapWidthSize * size) / 2.0f;// ステージのXを原点にするためのオフセット
			float offsetZ = (m_MapHeightSize * size) / 2.0f;// ステージのZを原点にするためのオフセット

			float leftX = (x * size) - offsetX;
			float rightX = ((x + 1) * size) - offsetX;
			float frontZ = (z * size) - offsetZ;
			float backZ = ((z + 1) * size) - offsetZ;

			float ground = 0.05f;// 床の高さ(0より少し高くする)

			int colorHandle = -1;
			int normalHandle = -1;
			
			if (m_2dMap[z][x] == 1)// ピンクの場合
			{
				colorHandle = m_pinkTextureHandle;
				normalHandle = m_nPinkTextureHandle;
			}
			else if (m_2dMap[z][x] == 2)// グリーンの場合
			{
				colorHandle = m_greenTextureHandle;
				normalHandle = m_nGreenTextureHandle;
			}
			if (colorHandle != -1 && normalHandle != -1)
			{
				if (m_InkShaderHandle != -1)
				{
					SetUsePixelShader(m_InkShaderHandle);
				}

				SetUseTextureToShader(0, colorHandle);
				SetUseTextureToShader(1, normalHandle);

				// 3Dポリゴンの頂点データ
				VERTEX3DSHADER vertices[6] =
				{
					// 三角形1
					{ { leftX,  ground,   backZ  }, {0.0f,1.0f,0.0f}, 0.0f, 0.0f},
					{ { rightX, ground,   backZ  }, {0.0f,1.0f,0.0f}, 1.0f, 0.0f},
					{ { leftX,  ground,   frontZ }, {0.0f,1.0f,0.0f}, 0.0f, 1.0f},
					// 三角形2
					{ { rightX, ground,   backZ  }, {0.0f,1.0f,0.0f}, 1.0f, 0.0f},
					{ { rightX, ground,   frontZ }, {0.0f,1.0f,0.0f}, 1.0f, 1.0f},
					{ { leftX,  ground,   frontZ }, {0.0f,1.0f,0.0f}, 0.0f, 1.0f}
				};

				DrawPolygon3DToShader(vertices,2);
				
			}
		}
	}
		// インクを書き終えたら元の描画にリセットする
	SetUsePixelShader(-1);
	SetUseAlphaTestFlag(FALSE);// アルファテストをOFFにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetUseBackCulling(TRUE);// カリングを元に戻す
}

void StageManager::Paint(float x, float z, float who)
{
	float offsetX = (m_MapWidthSize * m_cellSize) / 2.0f;// ステージのXを原点にするためのオフセット
	float offsetZ = (m_MapHeightSize * m_cellSize) / 2.0f;// ステージのZを原点にするためのオフセット

	int targetX = (x + offsetX) / m_cellSize;
	int targetZ = (z + offsetZ) / m_cellSize;

	if (targetX >= 0 && targetX < m_MapWidthSize && targetZ >= 0 && targetZ < m_MapHeightSize)
	{
		m_2dMap[targetZ][targetX] = who;
	}
	
}

