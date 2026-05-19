#pragma once
// ボムのクラス

class Bomb
{
public:
	Bomb();
	~Bomb();
	void Init();
	void Update();
	void Draw();
	// ボムを投げる
	void Throw();// ここにプレイヤーの位置を代入
};

