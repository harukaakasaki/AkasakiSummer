#pragma once
#include <DxLib.h>
#include <vector>
#include "Player.h"
class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Update(Vector3 playerPos);
	void Draw();

	float GetAngle() const { return m_cameraAngle; }

private:
	float m_cameraAngle;
	int m_skyModelHandle;// ‹ó‚Ìƒ‚ƒfƒ‹
	

	Vector3 m_cameraPos;
	Vector3 m_cameraTarget;
};

