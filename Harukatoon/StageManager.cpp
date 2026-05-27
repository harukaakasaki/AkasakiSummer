#include "StageManager.h"


StageManager::StageManager():
	m_2dMap(0),
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

	
}
void StageManager::Update()
{
	// プレイヤーの弾が当たった座標に色を塗る
}
void StageManager::Draw()
{

}

void StageManager::Paint(float x, float y, float who)
{

}

