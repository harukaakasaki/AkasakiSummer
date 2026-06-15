#include "StageManager.h"
#include <DxLib.h>
#include <cassert>

// 現状
// インクの塗っている場所をマスにして管理している
// 何したい？
// マス目上に描画されているため、インク同士を滑らかに重ね塗りができるようにしたい
// どうやる？
// 判定はそのままマス目上に管理して、見た目だけインクに変える（MakeScreen）
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

	// 一枚のインク書き込み用のキャンバスを作る
	m_inkCanvasHandle = MakeScreen(1024, 1024, TRUE);
	assert(m_inkCanvasHandle != -1);

	// 作ったキャンバスを一度透明でクリアにしておく
	SetDrawScreen(m_inkCanvasHandle);
	ClearDrawScreen();

	// 描画先を戻しておく
	SetDrawScreen(DX_SCREEN_BACK);

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
		SetUsePixelShader(m_inkShaderHandle);
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

			float ground = 0.5f;// 床の高さ(0より少し高くする)

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
				SetUseTextureToShader(0, colorHandle);// シェーダー側(t0)にインクの色
				SetUseTextureToShader(1, normalHandle);// シェーダー側(t1)にノーマルマップ

				COLOR_U8 white = GetColorU8(255, 255, 255, 255);

				// 3Dポリゴンの頂点データ
				VERTEX3DSHADER verticesShader[6];
				
				// --- 三角形1 ---
			    // 頂点0
				verticesShader[0].pos.x = leftX;  verticesShader[0].pos.y = ground; verticesShader[0].pos.z = backZ;
				verticesShader[0].norm.x = 0.0f;  verticesShader[0].norm.y = 1.0f; verticesShader[0].norm.z = 0.0f;
				verticesShader[0].u = 0.0f;        verticesShader[0].v = 0.0f;

				// 頂点1
				verticesShader[1].pos.x = rightX; verticesShader[1].pos.y = ground; verticesShader[1].pos.z = backZ;
				verticesShader[1].norm.x = 0.0f;  verticesShader[1].norm.y = 1.0f; verticesShader[1].norm.z = 0.0f;
				verticesShader[1].u = 1.0f;        verticesShader[1].v = 0.0f;

				// 頂点2
				verticesShader[2].pos.x = leftX;  verticesShader[2].pos.y = ground; verticesShader[2].pos.z = frontZ;
				verticesShader[2].norm.x = 0.0f;  verticesShader[2].norm.y = 1.0f; verticesShader[2].norm.z = 0.0f;
				verticesShader[2].u = 0.0f;        verticesShader[2].v = 1.0f;

				// --- 三角形2 ---
				// 頂点3
				verticesShader[3].pos.x = rightX; verticesShader[3].pos.y = ground; verticesShader[3].pos.z = backZ;
				verticesShader[3].norm.x = 0.0f;  verticesShader[3].norm.y = 1.0f; verticesShader[3].norm.z = 0.0f;
				verticesShader[3].u = 1.0f;        verticesShader[3].v = 0.0f;

				// 頂点4
				verticesShader[4].pos.x = rightX; verticesShader[4].pos.y = ground; verticesShader[4].pos.z = frontZ;
				verticesShader[4].norm.x = 0.0f;  verticesShader[4].norm.y = 1.0f; verticesShader[4].norm.z = 0.0f;
				verticesShader[4].u = 1.0f;        verticesShader[4].v = 1.0f;

				// 頂点5
				verticesShader[5].pos.x = leftX;  verticesShader[5].pos.y = ground; verticesShader[5].pos.z = frontZ;
				verticesShader[5].norm.x = 0.0f;  verticesShader[5].norm.y = 1.0f; verticesShader[5].norm.z = 0.0f;
				verticesShader[5].u = 0.0f;        verticesShader[5].v = 1.0f;
				
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

				DrawPolygon3DToShader(verticesShader,2);
				//DxLib::DrawPolygon3D(vertices, 2, colorHandle, true);
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

void StageManager::Paint(float x, float z, float who, float paintRadius)
{
	/*判定用*/
	// ステージの原点にするためのオフセット
	float offsetX = (m_mapWidthSize * m_cellSize) / 2.0f;
	float offsetZ = (m_mapHeightSize * m_cellSize) / 2.0f;

	// 弾の着弾点がなんマス目にあるのかを計算(ここを中心)
	int centerGridX = static_cast<int>((x + offsetX) / m_cellSize);
	int centerGridZ = static_cast<int>((z + offsetZ) / m_cellSize);

	// 半径が何マス分になるかの計算
	int radiusInCells = static_cast<int>(ceilf(paintRadius / m_cellSize));

	// 調べる範囲の開始と終了のマスを決める
	int startX = max(0, centerGridX - radiusInCells);
	int endX = min(m_mapWidthSize - 1, centerGridX + radiusInCells);
	int startZ = max(0, centerGridZ - radiusInCells);
	int endZ = min(m_mapHeightSize - 1, centerGridZ + radiusInCells);

	// 半径の2乗を計算
	float radiusSquart = paintRadius * paintRadius;

	// 決めた四角い範囲の中だけをループでチェックする
	for (int currentZ = startZ; currentZ <= endZ; ++currentZ)
	{
		for (int currentX = startX; currentX <= endX; ++currentX)
		{
			// 現在チェックしているマスの中心の3D座標を逆算
			float cellCenterX = (currentX * m_cellSize) + (m_cellSize / 2.0f)-offsetX;
			float cellCenterZ = (currentZ * m_cellSize) + (m_cellSize / 2.0f)-offsetZ;

			// 着弾点のマスと中心の距離の2乗を逆算
			float dx = cellCenterX - x;
			float dz = cellCenterZ - z;
			float distanceSquart = (dx * dx) + (dz * dz);

			// もし距離の2乗が半径の2乗以下なら円の内側のため、塗る。
			if (distanceSquart <= radiusSquart)
			{
				m_2dMap[currentZ][currentX] = who;
			}
		}
	}

	/*見た目用*/
	// 3D座標をキャンバス上の2Dピクセル座標に変換する
	float stageWidth = m_mapWidthSize * m_cellSize;
	float stageHeight = m_mapHeightSize * m_cellSize;

	int canvasX = static_cast<int>((x + offsetX) / stageWidth * 1024.0f);
	int canvasZ = static_cast<int>((z + offsetZ) / stageHeight * 1024.0f);

	// 描画先をインクのキャンバスに切り替える
	SetDrawScreen(m_inkCanvasHandle);

	// 通常の2Dブレンドモードにセット(インク画像をきれいに重ねる)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	
	// 塗ったプレイヤーによってインク画像のハンドルを変える
	int whoHandle = (who == 1.0f) ? m_orangeTextureHandle : m_blueTextureHandle;

	// 弾の塗る半径に合わせて、インクの描画サイズを計算する
	int inkCanvasSize = static_cast<int>((paintRadius * 2.0f) / stageWidth * 1024.0f);
	// もしインクサイズが小さすぎたら、16は描画されるようにする
	if (inkCanvasSize < 16)inkCanvasSize = 16;

	DrawExtendGraph(canvasX - inkCanvasSize / 2, canvasZ - inkCanvasSize / 2,
		            canvasX + inkCanvasSize / 2, canvasZ - inkCanvasSize / 2,
		            whoHandle, TRUE);

	SetDrawScreen(DX_SCREEN_BACK);
	
	/*int targetX = (x + offsetX) / m_cellSize;
	int targetZ = (z + offsetZ) / m_cellSize;

	if (targetX >= 0 && targetX < m_mapWidthSize && targetZ >= 0 && targetZ < m_mapHeightSize)
	{
		m_2dMap[targetZ][targetX] = who;
	}*/

}

