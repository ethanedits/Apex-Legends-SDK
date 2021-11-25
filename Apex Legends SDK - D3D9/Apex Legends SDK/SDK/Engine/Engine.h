#pragma once
#include <math.h>
#pragma warning( disable : 4244 )

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstddef>
#include <cstdint>
#include <cmath>
#define M_PI 3.14159265
#define RAD_TO_DEG(x) x * (180.f / M_PI)
#define DEG_TO_RAD(x) x * (M_PI / 180.f)

struct Matrix {
	float matrix[16];
};

struct Vector3 {
	float x, y, z;

	float distance(Vector3 vec)
	{
		return sqrt(
			pow(vec.x - x, 2) +
			pow(vec.y - y, 2)
		);
	}

	Vector3 operator-(Vector3 ape)
	{
		return { x - ape.x, y - ape.y, z - ape.z };
	}

	Vector3 operator+(Vector3 ape)
	{
		return { x + ape.x, y + ape.y, z + ape.z };
	}

	Vector3 operator*(float ape)
	{
		return { x * ape, y * ape, z * ape };
	}

	Vector3 operator/(float ape)
	{
		return { x / ape, y / ape, z / ape };
	}

	Vector3 operator/=(float ape)
	{
		x /= ape;
		y /= ape;
		z /= ape;

		return *this;
	}

	Vector3 operator+=(Vector3 ape)
	{
		return { x += ape.x, y += ape.y, z += ape.z };
	}

	Vector3 operator-=(Vector3 ape)
	{
		return { x -= ape.x, y -= ape.y, z -= ape.z };
	}

	void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}

	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float DistTo(Vector3 ape)
	{
		return (*this - ape).Length();
	}

	float Dist2D(Vector3 ape)
	{
		return (*this - ape).Length2D();
	}

	struct Vector3 ScreenPosition(struct Matrix matrix) {
		struct Vector3 out;
		float _x = matrix.matrix[0] * x + matrix.matrix[1] * y + matrix.matrix[2] * z + matrix.matrix[3];
		float _y = matrix.matrix[4] * x + matrix.matrix[5] * y + matrix.matrix[6] * z + matrix.matrix[7];
		out.z = matrix.matrix[12] * x + matrix.matrix[13] * y + matrix.matrix[14] * z + matrix.matrix[15];

		_x *= 1.f / out.z;
		_y *= 1.f / out.z;

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		out.x = width * .5f;
		out.y = height * .5f;

		out.x += 0.5f * _x * width + 0.5f;
		out.y -= 0.5f * _y * height + 0.5f;

		return out;
	}
};

struct Vector2 {
	float x, y;

	Vector2 operator-(Vector2 ape)
	{
		return { x - ape.x, y - ape.y };
	}

	Vector2 operator+(Vector2 ape)
	{
		return { x + ape.x, y + ape.y };
	}

	Vector2 operator*(float ape)
	{
		return { x * ape, y * ape };
	}

	Vector2 operator/(float ape)
	{
		return { x / ape, y / ape };
	}

	Vector2 operator/=(float ape)
	{
		x /= ape;
		y /= ape;

		return *this;
	}

	Vector2 operator+=(Vector2 ape)
	{
		return { x += ape.x, y += ape.y };
	}

	Vector2 operator-=(Vector2 ape)
	{
		return { x -= ape.x, y -= ape.y };
	}

	void Normalize()
	{
		if (x > 89.0f)
			x -= 180.f;

		if (x < -89.0f)
			x += 180.f;

		if (y > 180.f)
			y -= 360.f;

		if (y < -180.f)
			y += 360.f;
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float Dist2D(Vector2 ape)
	{
		return (*this - ape).Length2D();
	}
};

struct AimContext {
	int FOV;
	int aSmoothAmount;

	int crosshairX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int crosshairY = GetSystemMetrics(SM_CYSCREEN) / 2;

	int entX = 0;
	int entY = 0;

	int closestX = 0;
	int closestY = 0;

	int aX = 0;
	int aY = 0;

	float entNewVisTime = 0;
	float entOldVisTime[100];
	int visCooldownTime[100];
};

//Player Stuff
struct colorRGB
{
	float r, g, b;
};

struct glowMode
{
	BYTE GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

struct glowFade
{
	int a, b;
	float c, d, e, f;
};

struct Bone {
	BYTE thing[0xCC];
	float x;
	BYTE thing2[0xC];
	float y;
	BYTE thing3[0xC];
	float z;
};