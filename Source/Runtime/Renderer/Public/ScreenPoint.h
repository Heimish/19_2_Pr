#pragma once

#include "CoreDefinition.h"
#include "Vector2.h"

struct ScreenPoint
{
public:
	ScreenPoint() : X(0), Y(0) { }
	explicit ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }
	explicit ScreenPoint(float InX, float InY) : X(Math::RoundToInt(InX)), Y(Math::RoundToInt(InY)) {}
	explicit ScreenPoint(const Vector2& InPos) : X(Math::RoundToInt(InPos.X)), Y(Math::RoundToInt(InPos.Y)) {}
    
	FORCEINLINE ScreenPoint GetHalfSize() const
	{
		return ScreenPoint(Math::RoundToInt(X * 0.5f), Math::RoundToInt(Y * 0.5f));
	}

	FORCEINLINE Vector2 ToVector2() const
	{
		return Vector2((float)X + 0.5f, (float)Y + 0.5f);
	}


	int X;
	int Y;
};
