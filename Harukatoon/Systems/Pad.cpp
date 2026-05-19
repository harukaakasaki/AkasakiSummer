#include "Pad.h"
#include "DxLib.h"

namespace
{
	// 現在のフレームの押され状態
	int nowPad = 0;
	// 前のフレームの押され状態
	int lastPad = 0;
}

namespace Pad
{
	void Update()
	{
		// 前のフレームに取得したパッドの情報を入れ替え
		lastPad = nowPad;

		// 現在のフレームのパッドの情報を取得
		nowPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool IsPress(int key)
	{
		// このフレームに押されていればOK
		if (nowPad & key)	return true;

		return false;
	}

	bool IsTrigger(int key)
	{
		// このフレームに押されていない
		if (!(nowPad & key))	return false;

		// 前のフレームには押していない
		if (!(lastPad & key))	return true;

		// 前のフレームにも押していた(押しっぱなし)
		return false;
	}
}
