#pragma once

struct ScreenPoint
{
public:
	ScreenPoint() : X(0), Y(0) { }
	ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }
	//ScreenPoint(float InX, float InY) : X(Math::RoundToInt(InX)), Y(Math::RoundToInt(InY)) { }

	ScreenPoint GetHalfSize() const
	{
		return ScreenPoint((int)(X * 0.5f), (int)(Y * 0.5f));
	}

	int X;
	int Y;
};
