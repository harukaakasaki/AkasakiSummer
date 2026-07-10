#pragma once
#include "../GameObjects/Player.h"
// ボムのクラス

class Bomb
{
public:
	// コンストラクタ、デストラクタ
	Bomb();
	~Bomb();
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	// ボムを投げる
	void Throw();// ここにプレイヤーの位置を代入
};

