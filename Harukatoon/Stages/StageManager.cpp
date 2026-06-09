#include "StageManager.h"
#include <DxLib.h>
#include <cassert>

namespace
{
}

StageManager::StageManager() :
	m_mapWidthSize(0),
	m_mapHeightSize(0),
	m_cellSize(0),
	m_orangeTextureHandle(-1),
	m_blueTextureHandle(-1),
	m_nOrangeTextureHandle(-1),
	m_nBlueTextureHandle(-1),
	m_inkShaderHandle(-1)
{
}

StageManager::~StageManager()
{
}

void StageManager::Init()
{
	// プレイヤー1のインクテクスチャ
	m_orangeTextureHandle = LoadGraph("data/Ink/ink_orange.png");
	assert(m_orangeTextureHandle != -1);
	// プレイヤー2のインクテクスチャ
	m_blueTextureHandle = LoadGraph("data/Ink/ink_blue.png");
	assert(m_blueTextureHandle != -1);
	// プレイヤー1のノーマルマップ
	m_nOrangeTextureHandle = LoadGraph("data/Ink/ink_orange_n.png");
	assert(m_nOrangeTextureHandle != -1);
	// プレイヤー2のノーマルマップ
	m_nBlueTextureHandle = LoadGraph("data/Ink/ink_blue_n.png");
	assert(m_nBlueTextureHandle != -1);
	// インクのシェーダー
	m_inkShaderHandle = LoadPixelShader("InkShader.pso");
	assert(m_inkShaderHandle != -1);
	
	m_cellSize = 100.0f;
	m_mapWidthSize = 64;
	m_mapHeightSize = 48;

	m_2dMap.resize(m_mapHeightSize);// 縦48マスの空きを作る
	// メモリ内に48x64の紙を作成
	for (int i = 0; i < m_mapHeightSize; i++)
	{
		m_2dMap[i].resize(m_mapWidthSize, 0);
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
	if (m_inkShaderHandle != -1)
	{
		int result = SetUsePixelShader(m_inkShaderHandle);
	}

	// 3D空間にインクを描画する
	float size = m_cellSize;// インクの大きさ

	for (int z = 0; z < m_mapHeightSize; z++)
	{
		for (int x = 0; x < m_mapWidthSize; x++)
		{
			if (m_2dMap[z][x] == 0)
			{
				continue;
			}
			// 2Dのマス目を3D空間に変換して描画する
			float offsetX = (m_mapWidthSize * size) / 2.0f;// ステージのXを原点にするためのオフセット
			float offsetZ = (m_mapHeightSize * size) / 2.0f;// ステージのZを原点にするためのオフセット

			float leftX = (x * size) - offsetX;
			float rightX = ((x + 1) * size) - offsetX;
			float frontZ = (z * size) - offsetZ;
			float backZ = ((z + 1) * size) - offsetZ;

			float ground = 0.05f;// 床の高さ(0より少し高くする)

			int colorHandle = -1;
			int normalHandle = -1;

			if (m_2dMap[z][x] == 1)// プレイヤー1の場合
			{
				colorHandle = m_orangeTextureHandle;
				normalHandle = m_nOrangeTextureHandle;
			}
			else if (m_2dMap[z][x] == 2)// プレイヤー2の場合
			{
				colorHandle = m_blueTextureHandle;
				normalHandle = m_nBlueTextureHandle;
			}
			if (colorHandle != -1 && normalHandle != -1)
			{
				SetUseTextureToShader(0, colorHandle);
				SetUseTextureToShader(1, normalHandle);

				COLOR_U8 white = GetColorU8(255, 255, 255, 255);

				// 3Dポリゴンの頂点データ
				VERTEX3D vertices[6] =
				{
					// 三角形1
					{ { leftX,  ground,  backZ  }, {0.0f,1.0f,0.0f}, white, white, 0.0f, 0.0f },
					{ { rightX, ground,  backZ  }, {0.0f,1.0f,0.0f}, white, white, 1.0f, 0.0f },
					{ { leftX,  ground,  frontZ }, {0.0f,1.0f,0.0f}, white, white, 0.0f, 1.0f },
					// 三角形2
					{ { rightX, ground,  backZ  }, {0.0f,1.0f,0.0f}, white, white, 1.0f, 0.0f },
					{ { rightX, ground,  frontZ }, {0.0f,1.0f,0.0f}, white, white, 1.0f, 1.0f },
					{ { leftX,  ground,  frontZ }, {0.0f,1.0f,0.0f}, white, white, 0.0f, 1.0f }
				};

				//VERTEX3DSHADER verticesShader[6] =
				//{
				//	// 三角形1
				//	{ { leftX,  ground,  backZ  }, {0.0f,1.0f,0.0f}, 0.0f, 0.0f },
				//	{ { rightX, ground,  backZ  }, {0.0f,1.0f,0.0f},1.0f, 0.0f },
				//	{ { leftX,  ground,  frontZ }, {0.0f,1.0f,0.0f}, 0.0f, 1.0f },
				//	// 三角形2
				//	{ { rightX, ground,  backZ  }, {0.0f,1.0f,0.0f},  1.0f, 0.0f },
				//	{ { rightX, ground,  frontZ }, {0.0f,1.0f,0.0f}, 1.0f, 1.0f },
				//	{ { leftX,  ground,  frontZ }, {0.0f,1.0f,0.0f},0.0f, 1.0f }
				//};

				//DrawPolygon3DToShader(verticesShader,2);
				DxLib::DrawPolygon3D(vertices, 2, colorHandle, true);
				//DrawPolygon3D()

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
	float offsetX = (m_mapWidthSize * m_cellSize) / 2.0f;// ステージのXを原点にするためのオフセット
	float offsetZ = (m_mapHeightSize * m_cellSize) / 2.0f;// ステージのZを原点にするためのオフセット

	int targetX = (x + offsetX) / m_cellSize;
	int targetZ = (z + offsetZ) / m_cellSize;

	if (targetX >= 0 && targetX < m_mapWidthSize && targetZ >= 0 && targetZ < m_mapHeightSize)
	{
		m_2dMap[targetZ][targetX] = who;
	}

}

