#pragma once

#include "Vector2.h"
#include "LinearColor.h"

class Texture
{
public:
	Texture() { }
	Texture(char* resourceName);
	~Texture() { Release(); }

public:
	void LoadPNGFile(char *resourceName);
	void Release();
	bool IsIntialized() const { return bInitialized; }
	LinearColor* GetBuffer() const;
	LinearColor GetPixel(Vector2 InUV) const;
	UINT GetWidth() { return Width; }
	UINT GetHeight() { return Height; }

private:
	bool bInitialized = false;

	LinearColor* Buffer = nullptr;
	UINT Width = 0;
	UINT Height = 0;
};