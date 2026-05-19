#pragma once
#include <cmath>

class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	Vector3() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{
	}
	Vector3(float posX, float posY, float posZ) :
		x(posX),
		y(posY),
		z(posZ)
	{
	}
	// ’P€‰‰Zq+  
	Vector3 operator+() const
	{
		return *this;
	}
	// ’P€‰‰Zq-  
	Vector3 operator-() const
	{
		return Vector3{ -x,-y,-z };
	}

	// ‰ÁZ
	Vector3 operator+(const Vector3& vec) const
	{
		return Vector3{ x + vec.x, y + vec.y, z + vec.z };
	}
	// Œ¸Z
	Vector3 operator-(const Vector3& vec) const
	{
		return Vector3{ x - vec.x, y - vec.y, z - vec.z };
	}

	// +=
	Vector3 operator+=(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	// -=
	Vector3 operator-=(const Vector3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	// ƒXƒJƒ‰[”{i’è””{j
	Vector3 operator*(float scalar) const
	{
		return Vector3{ x * scalar, y * scalar, z * scalar };
	}

	// ’·‚³
	float Length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	// ³‹K‰»
	Vector3 Normalize() const
	{
		float length = Length();
		if (length == 0.0f)
		{
			return Vector3{ 0.0f, 0.0f, 0.0f }; // ’·‚³‚ªƒ[ƒ‚Ìê‡‚Íƒ[ƒƒxƒNƒgƒ‹‚ğ•Ô‚·
		}
		return Vector3(x / length, y / length, z / length);
	}
};

