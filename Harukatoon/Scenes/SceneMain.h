#pragma once
#include <DxLib.h>
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "ResultScene.h"

// プロトタイプ宣言
class Player;
class Camera;
class StageManager;
class CollisionManager;

class SceneMain :public Scene
{
public:
	SceneMain();
	~SceneMain();

	void Init()override;
	void Update()override;
	void Draw()override;

	bool IsEnd()const override;
	Scene* GetNextScene() override;

private:
	void DrawGrid();

	// ゲームシーンの状態
	enum class GameState
	{
		Playing,// ゲーム中
		Result  // 結果表示
	};

	GameState m_gameState = GameState::Playing;
	int m_timer;

private:
	int m_frameCount;
	int m_bgmHandle;        // BGMのハンドル
	int m_endSEHandle;      // 終了時のSEハンドル
	int m_gameUI;
	int m_reticleUI;        // レティクルUI
	int m_finish_1UI;       // フィニッシュUI一枚目
	int m_finish_2UI;       // フィニッシュUI二枚目
	int m_finish_3UI;       // フィニッシュUI三枚目
	int m_finish_4UI;       // フィニッシュUI四枚目
	int m_fontHandle;       // フォントのハンドル

	float m_finishScale;         // フィニッシュUIの大きさ
	float m_timeScale;
	float m_endTimer;            // 終了するまでのタイマー
	bool m_isFinish = false;     // 終了したかどうか
	bool m_isFinishAnim = false; // 終了UIのアニメーション

	WinnerType m_winnerState = WinnerType::None;

	// 各クラスのポインタ
	std::unique_ptr<Player> m_pPlayer1;
	std::unique_ptr<Player> m_pPlayer2;
	std::unique_ptr<Camera> m_pCamera1;
	std::unique_ptr<Camera> m_pCamera2;
	std::unique_ptr<StageManager> m_pStageManager;

	std::unique_ptr<CollisionManager> m_pCollisionManager;

	std::vector<Player*> m_pPlayerList;

};

