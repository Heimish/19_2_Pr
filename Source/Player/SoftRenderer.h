#pragma once

#include "WindowsRSI.h"

class SoftRenderer
{
public:
	enum class RenderMode
	{
		TWO,
		THREE_PERSP
	};

	SoftRenderer(SoftRenderer const&) = delete;
	void operator=(SoftRenderer const&) = delete;
	static SoftRenderer& Inst()
	{
		static SoftRenderer instance;
		return instance;
	}

public:
	void SetRenderMode(RenderMode InRenderMode) { CurrentRenderMode = InRenderMode; }
	RenderMode GetRenderMode() const { return CurrentRenderMode; }
	void Initialize();
	void Shutdown();
	void Update();
	void PreUpdate();
	void PostUpdate();
	float GetFrameFPS() const { return FrameFPS; }
	float GetAverageFPS() const { return AverageFPS; }
	float GetElapsedTime() const { return ElapsedTime; }
	int GetFrameCount() const { return FrameCount; }

	// Performace counter
	LONGLONG StartTimeStamp = 0;
	LONGLONG FrameTimeStamp = 0;
	long FrameCount = 0;
	float CyclesPerMilliSeconds = 0.f;
	float FrameTime = 0.f;
	float ElapsedTime = 0.f;
	float AverageFPS = 0.f;
	float FrameFPS = 0.f;
	double MilliSecFrequency = 0;
	double FrameMilliSec = 0;
	float FrameSec = 0;
	LONGLONG StartFrameSec = 0;

private:
	SoftRenderer() { }
	~SoftRenderer() { Shutdown(); }

	RenderMode CurrentRenderMode = RenderMode::TWO;

	RenderingSoftwareInterface* RSI = nullptr;
};
