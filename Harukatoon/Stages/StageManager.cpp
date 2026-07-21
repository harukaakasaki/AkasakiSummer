#include "StageManager.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	constexpr float kHandleScale = 4096.0f;
}

StageManager::StageManager() :
	m_mapWidthSize(0),
	m_mapHeightSize(0),
	m_cellSize(0),
	m_orangeTextureHandle(-1),
	m_blueTextureHandle(-1),
	m_nOrangeTextureHandle(-1),
	m_nBlueTextureHandle(-1),
	m_inkShaderHandle(-1),
	m_inkCanvasHandle(-1),
	m_inkNormalCanvasHandle(-1),
	m_stageModelHandle(-1)
{
}

StageManager::~StageManager()
{
	MV1DeleteModel(m_orangeTextureHandle);
	MV1DeleteModel(m_blueTextureHandle);
	MV1DeleteModel(m_blueTextureHandle);
	MV1DeleteModel(m_nBlueTextureHandle);
	MV1DeleteModel(m_inkShaderHandle);
	MV1DeleteModel(m_inkCanvasHandle);
	MV1DeleteModel(m_inkNormalCanvasHandle);
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

	// ステージのモデルをロード
	m_stageModelHandle = MV1LoadModel("data/Stage/stage_box.mv1");
	assert(m_stageModelHandle != -1);
	// ステージの位置とスケールを設定
	MV1SetPosition(m_stageModelHandle, VGet(0.0f, -100.0f, -1700.0f));

	m_cellSize = 100.0f;
	m_mapWidthSize = 128;
	m_mapHeightSize = 48;

	m_2dMap.resize(m_mapHeightSize);// 縦48マスの空きを作る
	// メモリ内に48x128の紙を作成
	for (int i = 0; i < m_mapHeightSize; i++)
	{
		m_2dMap[i].resize(m_mapWidthSize, 0);
	}

	// 一枚のインク書き込み用のキャンバスを作る
	m_inkCanvasHandle = MakeScreen(kHandleScale, kHandleScale, TRUE);
	assert(m_inkCanvasHandle != -1);
	// 書き込み用のキャンバスを作る
	m_inkNormalCanvasHandle = MakeScreen(kHandleScale, kHandleScale, TRUE);
	assert(m_inkNormalCanvasHandle != -1);

	// 作ったキャンバスを一度透明でクリアにしておく
	SetDrawScreen(m_inkCanvasHandle);
	ClearDrawScreen();
	// 書き込み用もクリアにする
	SetDrawScreen(m_inkNormalCanvasHandle);
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
	MV1DrawModel(m_stageModelHandle);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUseAlphaTestFlag(TRUE);// アルファテストをONにする（アルファ値が0の部分は描画されないようになる）
	SetUseBackCulling(FALSE);// 裏面も描画するようにする（これをしないと裏返ったポリゴンが見えなくなる）

	// インクの描画を開始（シェーダーON）
	if (m_inkShaderHandle != -1)
	{
		SetUsePixelShader(m_inkShaderHandle);
	}

	SetUseTextureToShader(0, m_inkCanvasHandle);
	SetUseTextureToShader(1, m_inkNormalCanvasHandle);

	// 2Dのマス目を3D空間に変換して描画する
	float totalWidth = m_mapWidthSize * m_cellSize;
	float totalHeight = m_mapHeightSize * m_cellSize;
	float offsetX = totalWidth / 2.0f;// ステージのXを原点にするためのオフセット
	float offsetZ = totalHeight / 2.0f;// ステージのZを原点にするためのオフセット

	float leftX = -offsetX;
	float rightX = offsetX;
	float frontZ = -offsetZ;
	float backZ = offsetZ;
	float ground = 0.5f;// 床の高さ(0より少し高くする)

	// 3Dポリゴンの頂点データ
	VERTEX3DSHADER verticesShader[6];

	// --- 三角形1 ---
	   // 頂点0
	verticesShader[0].pos.x = leftX;  verticesShader[0].pos.y = ground; verticesShader[0].pos.z = backZ;
	verticesShader[0].norm.x = 0.0f;  verticesShader[0].norm.y = 1.0f; verticesShader[0].norm.z = 0.0f;
	verticesShader[0].u = 0.0f;        verticesShader[0].v = 1.0f;

	// 頂点1
	verticesShader[1].pos.x = rightX; verticesShader[1].pos.y = ground; verticesShader[1].pos.z = backZ;
	verticesShader[1].norm.x = 0.0f;  verticesShader[1].norm.y = 1.0f; verticesShader[1].norm.z = 0.0f;
	verticesShader[1].u = 1.0f;        verticesShader[1].v = 1.0f;

	// 頂点2
	verticesShader[2].pos.x = leftX;  verticesShader[2].pos.y = ground; verticesShader[2].pos.z = frontZ;
	verticesShader[2].norm.x = 0.0f;  verticesShader[2].norm.y = 1.0f; verticesShader[2].norm.z = 0.0f;
	verticesShader[2].u = 0.0f;        verticesShader[2].v = 0.0f;

	// --- 三角形2 ---
	// 頂点3
	verticesShader[3].pos.x = rightX; verticesShader[3].pos.y = ground; verticesShader[3].pos.z = backZ;
	verticesShader[3].norm.x = 0.0f;  verticesShader[3].norm.y = 1.0f; verticesShader[3].norm.z = 0.0f;
	verticesShader[3].u = 1.0f;        verticesShader[3].v = 1.0f;

	// 頂点4
	verticesShader[4].pos.x = rightX; verticesShader[4].pos.y = ground; verticesShader[4].pos.z = frontZ;
	verticesShader[4].norm.x = 0.0f;  verticesShader[4].norm.y = 1.0f; verticesShader[4].norm.z = 0.0f;
	verticesShader[4].u = 1.0f;        verticesShader[4].v = 0.0f;

	// 頂点5
	verticesShader[5].pos.x = leftX;  verticesShader[5].pos.y = ground; verticesShader[5].pos.z = frontZ;
	verticesShader[5].norm.x = 0.0f;  verticesShader[5].norm.y = 1.0f; verticesShader[5].norm.z = 0.0f;
	verticesShader[5].u = 0.0f;        verticesShader[5].v = 0.0f;

	DrawPolygon3DToShader(verticesShader, 2);

	// インクを書き終えたら元の描画にリセットする
	SetUsePixelShader(-1);
	SetUseAlphaTestFlag(FALSE);// アルファテストをOFFにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetUseBackCulling(TRUE);// カリングを元に戻す
}

void StageManager::Paint(float x, float z, int who, float paintRadius)
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
			float cellCenterX = (currentX * m_cellSize) + (m_cellSize / 2.0f) - offsetX;
			float cellCenterZ = (currentZ * m_cellSize) + (m_cellSize / 2.0f) - offsetZ;

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

	int canvasX = static_cast<int>((x + offsetX) / stageWidth * kHandleScale);
	int canvasZ = static_cast<int>((z + offsetZ) / stageHeight * kHandleScale);

	// 描画先をインクのキャンバスに切り替える
	SetDrawScreen(m_inkCanvasHandle);

	// 通常の2Dブレンドモードにセット(インク画像をきれいに重ねる)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	// 塗ったプレイヤーによってインク画像のハンドルを変える(1の場合オレンジ2の場合ブルー)
	int colorHandle = (who == 1.0f) ? m_orangeTextureHandle : m_blueTextureHandle;
	int normalHandle = (who == 1.0f) ? m_nOrangeTextureHandle : m_nBlueTextureHandle;

	// 弾の塗る半径に合わせて、インクの描画サイズを計算する
	int inkCanvasSizeX = static_cast<int>((paintRadius * 2.0f) / stageWidth * kHandleScale);
	int inkCanvasSizeZ = static_cast<int>((paintRadius * 2.0f) / stageHeight * kHandleScale);
	// もしインクサイズが小さすぎたら、16は描画されるようにする
	if (inkCanvasSizeX < 16)inkCanvasSizeX = 16;
	if (inkCanvasSizeZ < 16)inkCanvasSizeZ = 16;

	SetDrawScreen(m_inkCanvasHandle);
	DrawExtendGraph(canvasX - inkCanvasSizeX / 2, canvasZ - inkCanvasSizeZ / 2,
		canvasX + inkCanvasSizeX / 2, canvasZ + inkCanvasSizeZ / 2,
		colorHandle, TRUE);

	// 現在のシェーダーは違和感があるため、シェーダーは止めている
	/*SetDrawScreen(m_inkNormalCanvasHandle);
	DrawExtendGraph(canvasX - inkCanvasSizeX / 2, canvasZ - inkCanvasSizeZ / 2,
					canvasX + inkCanvasSizeX / 2, canvasZ + inkCanvasSizeZ / 2,
					normalHandle, TRUE);*/

	SetDrawMode(DX_DRAWMODE_NEAREST);

	SetDrawScreen(DX_SCREEN_BACK);
}

float StageManager::GetPaintPercent(int playerColor) const
{
	int matchCount = 0;
	// 全体のマス数
	int totalCells = m_mapWidthSize * m_mapHeightSize;

	if (totalCells == 0)return 0.0f;

	for (const auto& row : m_2dMap)
	{
		for (const auto& cell : row)
		{
			if (static_cast<int>(cell) == playerColor)
			{
				matchCount++;
			}
		}
	}
	// プレイヤーの塗ったマス/全体のマス*100%=塗り割合(%)
	return (static_cast<float>(matchCount) / static_cast<float>(totalCells)) * 100.0f;
}

