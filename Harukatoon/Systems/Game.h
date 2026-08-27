#pragma once

// ゲーム全体で使用する定数
namespace Game
{
	// 画面全体の基本のサイズ
//	constexpr int kScreenWidth = 1280;
//  constexpr int kScreenHeight = 720;
	// 全画面表示用のサイズ
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;

	constexpr int kColorBitNum = 32;

	// 画面中央
	constexpr int kScreenCenterX = kScreenWidth / 2;
	constexpr int kScreenCenterY = kScreenHeight / 2;

	// 画面分割用のサイズ(2p用)
	constexpr int kSplitWidth = kScreenWidth / 2;
	constexpr int kSplitHeight = kScreenHeight;

	// 各画面のカメラ中心(スクリーン座標)
	constexpr float kCamera1CenterX = kSplitWidth / 2.0f;
	constexpr float kCamera2CenterX = kSplitWidth + (kSplitWidth / 2.0f);
	constexpr float kCameraCenterY = kScreenHeight / 2.0f;


}
