#pragma once
#include "CoreDefinition.h"

struct InputManager
{
	FORCEINLINE float GetXAxis() const
	{
		if (IsLeft ^ IsRight)
		{
			return IsLeft ? -1.0f : 1.0f;
		}

		return 0;
	}

	FORCEINLINE float GetYAxis() const
	{
		if (IsUp ^ IsDown)
		{
			return IsDown ? -1.0f : 1.0f;
		}

		return 0;
	}

	bool IsLeft = false;
	bool IsRight = false;
	bool IsUp = false;
	bool IsDown = false;
};
