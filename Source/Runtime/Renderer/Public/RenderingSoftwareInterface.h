
#pragma once

#include "DisplaySetting.h"
#include "LinearColor.h"

struct VertexData
{
	Vector2 Position;
	LinearColor Color = LinearColor::Error;
	Vector2 UV;
};

struct RSITexture
{
	RSITexture() = default;
	RSITexture(LinearColor* InTextureBuffer, UINT InWidth, UINT InHeight) : TextureBuffer(InTextureBuffer), Width(InWidth), Height(InHeight) {}
	UINT Width = 0;
	UINT Height = 0;
	LinearColor* TextureBuffer = nullptr;
};

class RenderingSoftwareInterface
{
public:
	virtual void Init(const bool InbSRGBColorSpace = true) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsIntialized() const = 0;
	virtual bool IsSRGBSpace() const = 0;

	virtual void Clear(const LinearColor& InClearColor) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor) = 0;
	virtual void DrawScreenPoint(const ScreenPoint& InScreenPos, const LinearColor& InColor) = 0;
	virtual void DrawVerticalLine(int InX, const LinearColor& InColor) = 0;
	virtual void DrawHorizontalLine(int InY, const LinearColor& InColor) = 0;

	virtual int SetTexture(RSITexture& InRSITexture) = 0;
	virtual void SetVertexBuffer(VertexData* InVertexData) = 0;
	virtual void SetIndexBuffer(const int* InIndexData) = 0;
	virtual void DrawPrimitive(UINT InVertexSize, UINT InIndexSize) = 0;

	LinearColor GetTextureSample(const Vector2& InUV);

private:
	RSITexture MainTexture;
};
