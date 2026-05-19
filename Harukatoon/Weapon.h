#pragma once
#include "Player.h"
// シューターのクラス

class Weapon
{
public:
	Weapon();
	~Weapon();
	void Init();
	void Update();
	void Draw();
	// ウェポンで撃つ関数
	// もしプレイヤーのインク容量がなくなると撃てなくなるようにしたい
	void Shot();// ここにプレイヤーの位置、向きを代入

private:
	
	float m_whoShot;// 誰が撃った？（プレイヤー1 or プレイヤー2）

	float m_shootTimer;// 連射速度を制御する

};

