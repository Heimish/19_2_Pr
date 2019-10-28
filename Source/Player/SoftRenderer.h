#pragma once

#include "WindowsRSI.h"
#include "InputManager.h"
#include "Texture.h"

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
	InputManager& GetInputManager() { return InputManager; }

	float GetFrameFPS() const { return FrameFPS; }
	float GetAverageFPS() const { return AverageFPS; }

	void Initialize();
	void Shutdown();
	void PreUpdate();
	void Update();
	void PostUpdate();

private:
	SoftRenderer() { }
	~SoftRenderer() { Shutdown(); }

	RenderMode CurrentRenderMode = RenderMode::TWO;

	RenderingSoftwareInterface* RSI = nullptr;

	// Performace counter
	LONGLONG StartTimeStamp = 0;
	LONGLONG FrameTimeStamp = 0;
	long FrameCount = 0;
	float CyclesPerMilliSeconds = 0.f;
	float FrameTime = 0.f;
	float ElapsedTime = 0.f;
	float AverageFPS = 0.f;
	float FrameFPS = 0.f;

	InputManager InputManager;
	Texture MainTexture;
};
