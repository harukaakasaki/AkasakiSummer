#include "Pad.h"
#include "DxLib.h"

namespace
{
	constexpr int kPadCount = 5;
	// 現在のフレームの押され状態
	int nowPad[kPadCount] = {};
	// 前のフレームの押され状態
	int lastPad[kPadCount] = {};
}

namespace Pad
{
	void Update()
	{
		for (int i = 0; i < kPadCount; i++)
		{
			// 前のフレームに取得したパッドの情報を入れ替え
			lastPad[i] = nowPad[i];

			// 現在のフレームのパッドの情報を取得
			nowPad[i] = GetJoypadInputState(i);
		}

	}

	bool IsPress(int padNo, int key)
	{
		// このフレームに押されていればOK

		return (nowPad[padNo] & key) != 0;
	}

	bool IsTrigger(int padNo, int key)
	{
		// このフレームに押されていない
		if (!(nowPad[padNo] & key))	return false;

		// 前のフレームには押していない
		if (!(lastPad[padNo] & key))	return true;

		// 前のフレームにも押していた(押しっぱなし)
		return false;
	}
}