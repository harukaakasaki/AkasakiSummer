#include "Animation.h"
#include <DxLib.h>

Animation::Animation() :
	m_modelHandle(-1),
	m_currentAnimIndex(-1),
	m_currentAnimHandle(-1),
	m_lastAnimHandle(-1),
	m_currentAnimCount(0.0f),
	m_lastAnimCount(0.0f),
	m_changeAnimFrame(0),
	m_animSpeed(0.0f),
	m_isLoop(),
	m_isEnd(),
	m_globalSpeed(1.0f)
{
}

Animation::~Animation()
{
}

void Animation::Init(int modelHandle)
{
	m_modelHandle = modelHandle;

	m_currentAnimHandle = -1;
	m_currentAnimIndex = -1;
	m_lastAnimHandle = -1;

	m_currentAnimCount = 0.0f;
	m_lastAnimCount = 0.0f;

	m_changeAnimFrame = 0;
	m_globalSpeed = 1.0f;

	m_isLoop = true;

	m_isEnd = false;
}

void Animation::Update()
{
	if (m_currentAnimHandle == -1)return;

	// ブレンドする
	m_changeAnimFrame++;

	// ブレンド率を計算する（徐々に新しいアニメに切り替え）
	float blendRate = (float)m_changeAnimFrame / 10.0f; // 10フレームかけて切り替える
	if (blendRate > 1.0f)blendRate = 1.0f;

	// アニメーション同士をブレンドする
	if (m_lastAnimHandle != -1)
	{
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimHandle, blendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_lastAnimHandle, 1.0f - blendRate);
	}
	// 前のアニメーションがなかった場合
	else
	{
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimHandle, 1.0f);
	}

	// アニメーションの再生時間を更新する
	m_currentAnimCount += m_animSpeed * m_globalSpeed;

	// アニメーションの長さを取得する
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, m_currentAnimHandle);

	// アニメーションが最後まで再生されたら
	if (m_currentAnimCount >= total)
	{
		// ループするかどうかで処理を分ける
		if (m_isLoop)
		{
			m_currentAnimCount = 0.0f;
		}

		else
		{
			// アニメーションが終了したら最後のフレームで止める
			m_currentAnimCount = total;
			m_isEnd = true;
		}

	}
	// アニメーションの反映
	MV1SetAttachAnimTime(m_modelHandle, m_currentAnimHandle, m_currentAnimCount);
}

// アニメーションを再生する
void Animation::Play(int animIndex, bool loop, float speed)
{
	// すでに再生中のアニメーションと同じ場合は何もしない
	if (m_currentAnimIndex == animIndex)
	{
		m_animSpeed = speed;
		return;
	}
	m_currentAnimIndex = animIndex;

	// 前のアニメーションを削除
	if (m_lastAnimHandle != -1)
	{
		MV1DetachAnim(m_modelHandle, m_lastAnimHandle);
	}

	// 新しいアニメーションを再生
	m_lastAnimHandle = m_currentAnimHandle;
	m_lastAnimCount = m_currentAnimCount;

	// アニメーションをアタッチする
	m_currentAnimHandle = MV1AttachAnim(m_modelHandle, animIndex, -1, -1);
	m_currentAnimCount = 0.0f;

	m_isLoop = loop;

	m_isEnd = false;

	m_animSpeed = speed;

	m_changeAnimFrame = 0;
}

// アニメーションを停止させる
void Animation::Stop()
{
	if (m_currentAnimHandle != -1)
	{
		MV1DetachAnim(m_modelHandle, m_currentAnimHandle);
		m_currentAnimHandle = -1;
	}

	if (m_lastAnimHandle != -1)
	{
		MV1DetachAnim(m_modelHandle, m_lastAnimHandle);
		m_lastAnimHandle = -1;
	}

	m_currentAnimIndex = -1;
}

void Animation::SetGlobalSpeed(float speed)
{
	m_globalSpeed = speed;
}

// アニメーションが再生中かどうか
bool Animation::IsPlaying() const
{
	return m_currentAnimHandle != -1;
}

bool Animation::IsEnd() const
{
	return m_isEnd;
}