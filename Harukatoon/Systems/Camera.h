#pragma once
#include <DxLib.h>
#include <vector>
#include "../GameObjects/Player.h"
class Camera final
{
public:
	// コンストラクタ
	Camera();
	// デストラクタ
	~Camera();

	/// <summary>
	/// カメラのコピーコンストラクタを削除
	/// </summary>
	/// <param name=""></param>
	Camera(const Camera&) = delete;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="padNo"></param>
	void Init(int padNo);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="playerPos"></param>
	void Update(VECTOR playerPos);

	/// <summary>
	///  描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// カメラの横回転を取得
	/// </summary>
	/// <returns></returns>
	float GetYaw() const { return m_cameraYaw; }

	/// <summary>
	/// カメラの縦回転を取得
	/// </summary>
	/// <returns></returns>
	float GetPitch() const { return m_cameraPitch; }

private:
	float m_cameraYaw;     // 横回転
	float m_cameraPitch;   // 縦回転

	int m_skyModelHandle;  // 空のモデル
	int m_padNo;           // パッド番号

	VECTOR m_cameraPos;    // カメラの位置
	VECTOR m_cameraTarget; // カメラの注視点
};

