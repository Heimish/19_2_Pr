
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "TriangleRasterizer.h"

void WindowsRSI::Init(const bool InbSRGBColorSpace)
{
	InitializeGDI(DisplaySetting::Inst().GetSize(), InbSRGBColorSpace);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	SetColor(InClearColor);
	FillBuffer();

	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{

}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawScreenPoint(const ScreenPoint & InPoint, const LinearColor & InColor)
{
	PutPixel(InPoint, InColor.ToColor32());
}

int WindowsRSI::SetTexture(RSITexture & InRSITexture)
{
	MainTexture.TextureBuffer = InRSITexture.TextureBuffer;
	MainTexture.Width = InRSITexture.Width;
	MainTexture.Height = InRSITexture.Height;
	HasTexture = true;
	return 0;
}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	IndexBuffer = InIndexData;
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
	{
		return;
	}

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		//VertexData tv[3] = { 
		//	VertexBuffer[IndexBuffer[ti * 3]], 
		//	VertexBuffer[IndexBuffer[ti * 3 + 1]], 
		//	VertexBuffer[IndexBuffer[ti * 3 + 2]] 
		//};
		//VertexData temp;

		//// TO-DO. 퇴화 삼각형인지 식별.

		//// 1. 삼각형 정점 소팅
		//// 1-1. 0번과 1번의 Y값 비교
		//if (tv[0].Position.Y == tv[1].Position.Y)
		//{
		//	// X 값을 비교.
		//	if (tv[0].Position.X > tv[1].Position.X)
		//	{
		//		// 0번과 1번을 Swap
		//		temp = tv[0];
		//		tv[0] = tv[1];
		//		tv[1] = temp;
		//	}
		//}
		//else
		//{
		//	if (tv[0].Position.Y < tv[1].Position.Y)
		//	{
		//		// 0번과 1번을 Swap
		//		temp = tv[0];
		//		tv[0] = tv[1];
		//		tv[1] = temp;
		//	}
		//}

		//if (tv[1].Position.Y == tv[2].Position.Y)
		//{
		//	// X 값을 비교.
		//	if (tv[1].Position.X > tv[2].Position.X)
		//	{
		//		// 1번과 2번을 Swap
		//		temp = tv[1];
		//		tv[1] = tv[2];
		//		tv[2] = temp;
		//	}
		//}
		//else
		//{
		//	if (tv[1].Position.Y < tv[2].Position.Y)
		//	{
		//		// 1번과 2번을 Swap
		//		temp = tv[1];
		//		tv[1] = tv[2];
		//		tv[2] = temp;
		//	}
		//}


		//if (tv[0].Position.Y == tv[1].Position.Y)
		//{
		//	// X 값을 비교.
		//	if (tv[0].Position.X > tv[1].Position.X)
		//	{
		//		// 0번과 1번을 Swap
		//		temp = tv[0];
		//		tv[0] = tv[1];
		//		tv[1] = temp;
		//	}
		//}
		//else
		//{
		//	if (tv[0].Position.Y < tv[1].Position.Y)
		//	{
		//		// 0번과 1번을 Swap
		//		temp = tv[0];
		//		tv[0] = tv[1];
		//		tv[1] = temp;
		//	}
		//}

		//// 2. 삼각형 패턴 파악 ( Top-Flat , Bottom-Flat, Normal )

		//if (tv[0].Position.Y == tv[1].Position.Y)
		//{
		//	DrawTopFlatTriangle(tv);
		//}
		//else if (tv[1].Position.Y == tv[2].Position.Y)
		//{
		//	DrawBottomFlatTriangle(tv);
		//}
		//else
		//{
		//	// 삼각형을 두 개로 쪼갠다.
		//	VertexData newV = tv[1];
		//	float height = tv[0].Position.Y - tv[2].Position.Y;
		//	float width = tv[2].Position.X - tv[0].Position.X;

		//	if (height == 0.0f)
		//	{
		//		return;
		//	}

		//	float gradient = width / height;
		//	newV.Position.X = gradient * (tv[0].Position.Y - tv[1].Position.Y) + tv[0].Position.X;

		//	if (newV.Position.X > tv[1].Position.X)
		//	{
		//		VertexData upperTriangle[3] = { tv[0], tv[1], newV };
		//		VertexData bottomTriangle[3] = { tv[1], newV, tv[2] };
		//		DrawTopFlatTriangle(bottomTriangle);
		//		DrawBottomFlatTriangle(upperTriangle);
		//	}
		//	else
		//	{
		//		VertexData upperTriangle[3] = { tv[0], newV, tv[1] };
		//		VertexData bottomTriangle[3] = { newV, tv[1], tv[2] };
		//		DrawTopFlatTriangle(bottomTriangle);
		//		DrawBottomFlatTriangle(upperTriangle);
		//	}
		//}

		// Draw Each Triangle List 
		TriangleRasterizer t(
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]);
		t.RecalcBounds();
		for (int x = t.TopLeft.X; x < t.BottomRight.X; ++x)
		{
			for (int y = t.TopLeft.Y; y < t.BottomRight.Y; ++y)
			{
				ScreenPoint currentPixel(x, y);
				Vector2 currentPos = currentPixel.ToVector2();
				if (t.IsInside(currentPos))
				{
					if (HasTexture)
					{
						PutPixel(currentPixel, GetTextureSample(t.GetUV(currentPos)));
					}
					else
					{
						PutPixel(currentPixel, t.GetColor(currentPos));
					}
				}
			}
		}

	}
}

void WindowsRSI::DrawLine(const Vector2 & InStartPos, const Vector2 & InEndPos, const LinearColor & InColor)
{
	ScreenPoint startPos(InStartPos);
	ScreenPoint endPos(InEndPos);

	int w = Math::Abs(endPos.X - startPos.X);
	int h = Math::Abs(endPos.Y - startPos.Y);
	int dx = endPos.X > startPos.X ? 1 : -1;
	int dy = endPos.Y > startPos.Y ? 1 : -1;

	if (w < 0) return;
	if (h < 0) return;

	int f = (h <= w) ? 2 * h - w : h - 2 * w;
	int f1 = (h <= w) ? 2 * h : -2 * w;
	int f2 = (h <= w) ? 2 * (h - w) : -2 * (w - h);

	int y = startPos.Y;
	int x = startPos.X;

	while (true)
	{
		DrawScreenPoint(ScreenPoint(x, y), InColor);

		if (h <= w)
		{
			if (x == endPos.X)
			{
				break;
			}
		}
		else
		{
			if (y == endPos.Y)
			{
				break;
			}
		}

		if (h <= w)
		{
			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				y += dy;
			}

		}
		else
		{
			if (f > 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				x += dx;
			}
		}

		if (h <= w)
		{
			x += dx;
		}
		else
		{
			y += dy;
		}
	}
}

void WindowsRSI::DrawVerticalLine(int InX, const LinearColor & InColor)
{
	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;

	int xIndex = InX + Math::FloorToInt(((float)ScreenSize.X - 1.f) * 0.5f);
	for (int y = 0; y < ScreenSize.Y; ++y)
	{
		*(dest + xIndex) = color;
		dest += ScreenSize.X;
	}
}

void WindowsRSI::DrawHorizontalLine(int InY, const LinearColor & InColor)
{
	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;
	int startIndex = Math::FloorToInt(((float)ScreenSize.Y - 1.f) * 0.5f) - InY;
	dest = dest + startIndex * ScreenSize.X;
	for (int x = 0; x < ScreenSize.X; x++)
	{
		*dest = color;
		dest++;
	}
}

void WindowsRSI::DrawTopFlatTriangle(VertexData * tvs, bool DrawLastLine)
{
	float dx1 = tvs[0].Position.X - tvs[2].Position.X;
	float dx2 = tvs[1].Position.X - tvs[2].Position.X;
	float dy = tvs[2].Position.Y - tvs[1].Position.Y;

	if (dy >= 0)
	{
		return;
	}

	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	PutPixel(ScreenPoint(tvs[2].Position), LinearColor(1.f, 0.f, 0.f));
	float startY = tvs[2].Position.Y;
	float startX = tvs[2].Position.X;
	float currentY = floorf(tvs[2].Position.Y) - 0.5f;
	float destY = tvs[1].Position.Y;
	while (currentY <= destY)
	{
		float deltaY = startY - currentY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int pixelX1 = Math::FloorToInt(leftX);
		int pixelX2 = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
		currentY += 1.0f;
	}

	if (DrawLastLine)
	{
		// 마지막 라인을 그린다.
		int pixelX1 = Math::FloorToInt(tvs[1].Position.X);
		int pixelX2 = Math::FloorToInt(tvs[2].Position.X);
		int pixelY = Math::FloorToInt(destY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
	}
}

void WindowsRSI::DrawBottomFlatTriangle(VertexData * tvs)
{
	float dx1 = tvs[1].Position.X - tvs[0].Position.X;
	float dx2 = tvs[2].Position.X - tvs[0].Position.X;
	float dy = tvs[0].Position.Y - tvs[1].Position.Y;

	if (dy <= 0)
	{
		return;
	}

	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	PutPixel(ScreenPoint(tvs[0].Position), LinearColor(0.f, 1.f, 0.f));
	float startY = tvs[0].Position.Y;
	float startX = tvs[0].Position.X;
	float currentY = floorf(tvs[0].Position.Y) - 0.5f;
	float destY = tvs[1].Position.Y;
	while (currentY >= destY)
	{
		float deltaY = startY - currentY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int startX = Math::FloorToInt(leftX);
		int endX = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = startX; p <= endX; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
		}
		currentY -= 1.0f;
	}

	// 마지막 라인을 그린다.
	int pixelX1 = Math::FloorToInt(tvs[0].Position.X);
	int pixelX2 = Math::FloorToInt(tvs[1].Position.X);
	int pixelY = Math::FloorToInt(destY);
	for (int p = pixelX1; p <= pixelX2; ++p)
	{
		PutPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
	}
}

LinearColor WindowsRSI::GetTextureSample(const Vector2 & InUV)
{
	UINT width = MainTexture.Width;
	UINT height = MainTexture.Height;
	UINT pixelX = Math::FloorToInt(InUV.X * width);
	UINT pixelY = Math::FloorToInt(InUV.Y * height);
	pixelX %= width;
	pixelY %= height;
	UINT textureIndex = width * pixelY + pixelX;
	return MainTexture.TextureBuffer[textureIndex];
}