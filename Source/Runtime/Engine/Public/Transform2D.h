#pragma once

#include "Matrix3x3.h"

class Transform2D
{
public:
	Matrix3x3 GetTRS() const;
	void SetPosition(const Vector2& InPosition);
	Vector2 GetPosition() { return Position; }

	void SetRotation(float InEulerRotationInDegree);
	void SetScale(const Vector2& InScale);

	void AddPosition(const Vector2& InPosition);
	void AddRotation(float InRotationDegree);
	void AddScale(const Vector2& InScale);

private:
	Vector2 Position = Vector2::Zero;
	float RotationInDegree = 0.f;
	Vector2 Scale = Vector2::One;
};