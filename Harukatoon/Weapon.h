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
	void Shot();// ここにプレイヤーの位置を代入

private:


};

