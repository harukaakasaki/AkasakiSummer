#pragma once
#include <vector>
#include <memory>
#include "Bullet.h"
#include "../GameObjects/Player.h"
#include "../Stages/StageManager.h"
// ウェポンクラスはバレットクラスです！

class StageManager;

class Weapon
{
public:
	// コンストラクタ、デストラクタ
	Weapon(StageManager* stageManager, int playerColor);
	~Weapon();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 弾のポインタの取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::unique_ptr<Bullet>>& GetBullets() { return m_bullets; }

	/// <summary>
	/// 生きている弾の取得
	/// </summary>
	std::vector<std::unique_ptr<Bullet>>aliveBullets;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	// ウェポンで撃つ関数
	// もしプレイヤーのインク容量がなくなると撃てなくなるようにしたい
	void UseWeapon(VECTOR playerPos, VECTOR shotVel);// ウェポンを使ったらバレットクラスに伝える

private:
	// 弾の動的配列
	std::vector<std::unique_ptr<Bullet>>m_bullets;

	// Stageマネージャーのポインタ
	StageManager* m_stageManager;

	// 誰が撃った？（プレイヤー1 or プレイヤー2）
	float m_whoShot;

	// 連射速度を制御する
	int m_shotTimer;

	// 弾と弾の間
	int m_shotInterval;

	// プレイヤーの色
	int m_playerColor;

	//メインの弾かどうか（遠くに飛ぶ弾かどうか）
	bool m_isMainBullet;
	//連続で打ったフレームをカウントする
	int m_shootingCountFrame;
};