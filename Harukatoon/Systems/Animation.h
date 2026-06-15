#pragma once
class Animation
{
public:
	Animation();
	~Animation();
	void Init(int modelHandle);
	void Update();
	void Play(int animIndex, bool loop, float speed);
	void Stop();
	void SetGlobalSpeed(float speed);

	bool IsPlaying() const;
	bool IsEnd() const;

private:
	// アニメーションモデルのハンドル
	int m_modelHandle;

	// 現在再生中のアニメーションのインデックス
	int m_currentAnimIndex;

	// 現在再生中のアニメーションのハンドル
	int m_currentAnimHandle;

	// 最後に再生したアニメーションのインデックス
	int m_lastAnimHandle;

	// アニメーションの再生時間
	float m_currentAnimCount;

	// 最後に再生したアニメーションの再生時間
	float m_lastAnimCount;

	// アニメーションの再生速度
	float m_globalSpeed;

	// アニメーションを切り替えるフレーム数
	int m_changeAnimFrame;

	// アニメーションの再生速度
	int m_animSpeed;

	// アニメーションをループさせるかどうか
	bool m_isLoop;

	// アニメーションが終了したかどうか
	bool m_isEnd;

};