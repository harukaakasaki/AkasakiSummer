#pragma once
#include <DxLib.h>
#include <vector>
#include "../GameObjects/Player.h"
class Camera final
{
public:
	Camera();
	~Camera();

	Camera(const Camera&)=delete;
	void Init(int padNo);
	void Update(VECTOR playerPos);
	void Draw();

	float GetYaw() const { return m_cameraYaw; }
	float GetPitch() const { return m_cameraPitch; }

private:
	float m_cameraYaw; // ‰¡‰ñ“]
	float m_cameraPitch; // c‰ñ“]

	int m_skyModelHandle;// ‹ó‚Ìƒ‚ƒfƒ‹
	int m_padNo; // ƒpƒbƒh”Ô†

	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

