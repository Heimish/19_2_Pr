#pragma once
#include "ScreenPoint.h"

class DisplaySetting
{
public:
	DisplaySetting(DisplaySetting const&) = delete;
	void operator=(DisplaySetting const&) = delete;
	
	static DisplaySetting& Inst()
	{
		static DisplaySetting inst;
		return inst;
	}

public:
	void SetSize(ScreenPoint InSize)
	{
		Size = InSize;
	}

	ScreenPoint GetSize() const
	{
		return Size;
	}

private:
	DisplaySetting() {}

	ScreenPoint Size;
	
};