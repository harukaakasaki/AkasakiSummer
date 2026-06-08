#pragma once
#include <vector>
#include <memory>
#include "Bullet.h"
#include "../GameObjects/Player.h"
// ウェポンクラスはバレットクラスです！

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
	void UseWeapon(VECTOR playerPos,VECTOR shotVel);// ウェポンを使ったらバレットクラスに伝える

private:
	// 弾の動的配列
	std::vector<std::unique_ptr<Bullet>>m_bullets;
	
	float m_whoShot;// 誰が撃った？（プレイヤー1 or プレイヤー2）

	float m_shootTimer;// 連射速度を制御する

};

