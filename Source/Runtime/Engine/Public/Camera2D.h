#pragma once

#include "Transform2D.h"
#include "Matrix3x3.h"

class Camera2D
{
public:
	Camera2D() {}

	Transform2D& GetTransform();
	Matrix3x3 GetViewMatrix();

public:
	Transform2D Transform;
};