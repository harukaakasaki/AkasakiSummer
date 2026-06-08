#pragma once
#include <DxLib.h>
#include <vector>
#include "../GameObjects/Player.h"
class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Update(VECTOR playerPos);
	void Draw();

	float GetAngle() const { return m_cameraAngle; }

private:
	float m_cameraAngle;
	int m_skyModelHandle;// ‹ó‚Ìƒ‚ƒfƒ‹
	

	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

