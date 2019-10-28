#pragma once

#include "Transform2D.h"

class GameObject2D
{
public:
	GameObject2D() {}

	Transform2D& GetTransform();

public:
	Transform2D Transform;
};
