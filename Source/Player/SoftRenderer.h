#pragma once

class SoftRenderer
{
public:
	SoftRenderer(SoftRenderer const&) = delete;
	void operator=(SoftRenderer const&) = delete;
	static SoftRenderer& Inst()
	{
		static SoftRenderer instance;
		return instance;
	}

public:
	void Initialize(HWND InWnd);
	void Shutdown();
	void Update();

public:

private:
	SoftRenderer() { }
	~SoftRenderer() { Shutdown(); }
};
