#pragma once

namespace Pad
{
	// 毎フレームのアップデート処理
	// 1フレームに1回だけ呼び出す
	void Update();

	// ボタンが押されているかを取得
	bool IsPress(int padNo,int key);

	// ボタンが押された瞬間を取得
	bool IsTrigger(int padNo,int key);

}
