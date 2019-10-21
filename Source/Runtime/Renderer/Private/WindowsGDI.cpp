
#include "Precompiled.h"
#include "WindowsGDI.h"

void WindowsGDI::InitializeGDI(const ScreenPoint& InDisplaySize, const bool InbSRGBColorSpace)
{
	Wnd = ::GetActiveWindow();
	if (Wnd == NULL)
	{
		return;
	}

	if (bGDIInitialized)
	{
		DeleteObject(DefaultBitmap);
		DeleteObject(DIBitmap);
		ReleaseDC(Wnd, ScreenDC);
		ReleaseDC(Wnd, MemoryDC);
	}

	ScreenDC = GetDC(Wnd);
	if (ScreenDC == NULL)
	{
		return;
	}

	MemoryDC = CreateCompatibleDC(ScreenDC);
	if (MemoryDC == NULL)
	{
		return;
	}

	ScreenSize = InDisplaySize;
	bSRGBColorSpace = InbSRGBColorSpace;

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = ScreenSize.X;
	bmi.bmiHeader.biHeight = -ScreenSize.Y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	DIBitmap = CreateDIBSection(MemoryDC, &bmi, DIB_RGB_COLORS, (void**)&ScreenBuffer, NULL, 0);
	if (DIBitmap == NULL)
	{
		return;
	}

	DefaultBitmap = (HBITMAP)SelectObject(MemoryDC, DIBitmap);
	if (DefaultBitmap == NULL)
	{
		return;
	}

	bGDIInitialized = true;
}

void WindowsGDI::ReleaseGDI()
{
	if (bGDIInitialized)
	{
		DeleteObject(DefaultBitmap);
		DeleteObject(DIBitmap);
		ReleaseDC(Wnd, ScreenDC);
		ReleaseDC(Wnd, MemoryDC);
	}

	if (DepthBuffer != nullptr)
	{
		delete[] DepthBuffer;
		DepthBuffer = nullptr;
	}

	bGDIInitialized = false;
}


void WindowsGDI::FillBuffer()
{
	if (!bGDIInitialized || (ScreenBuffer == NULL))
	{
		return;
	}

	Color32* dest = ScreenBuffer;
	unsigned long totalCount = ScreenSize.X * ScreenSize.Y;
	while (totalCount--)
	{
		*dest++ = CurrentColor;
	}

	return;
}

void WindowsGDI::SetColor(const LinearColor& InColor)
{
	CurrentColor = InColor.ToColor32(bSRGBColorSpace);
}

LinearColor WindowsGDI::GetPixel(const ScreenPoint& InPos)
{
	if (!IsInScreen(InPos))
	{
		return LinearColor::Error;
	}

	ScreenPoint halfSize = ScreenSize.GetHalfSize();
	Color32* dest = ScreenBuffer;
	long offset = ScreenSize.X * halfSize.Y + halfSize.X + InPos.X + ScreenSize.X * -InPos.Y;
	Color32 bufferColor = *(dest + offset);
	return LinearColor(bufferColor);
}

void WindowsGDI::PutPixel(const ScreenPoint& InPos)
{
	PutPixel(InPos, CurrentColor);
}

FORCEINLINE bool WindowsGDI::IsInScreen(const ScreenPoint& InPos) const
{
	ScreenPoint halfSize = ScreenSize.GetHalfSize();
	if (!(Math::Abs(InPos.X) < halfSize.X) || !(Math::Abs(InPos.Y) < halfSize.Y))
	{
		return false;
	}

	return true;
}

void WindowsGDI::PutPixel(const ScreenPoint& InPos, const Color32& InColor)
{
	if (!IsInScreen(InPos))
	{
		return;
	}

	ScreenPoint halfSize = ScreenSize.GetHalfSize();
	Color32* dest = ScreenBuffer;
	long offset = ScreenSize.X * halfSize.Y + halfSize.X + InPos.X + ScreenSize.X * -InPos.Y;
	*(dest + offset) = InColor;
	return;
}


Color32* WindowsGDI::GetScreenBuffer() const
{
	return ScreenBuffer;
}


void WindowsGDI::SwapBuffer()
{
	if (!bGDIInitialized)
	{
		return;
	}

	BitBlt(ScreenDC, 0, 0, ScreenSize.X, ScreenSize.Y, MemoryDC, 0, 0, SRCCOPY);
}


void WindowsGDI::PutPixel(const ScreenPoint& InPos, const LinearColor& InColor)
{
	PutPixel(InPos, InColor.ToColor32(bSRGBColorSpace));
}

void WindowsGDI::CreateDepthBuffer()
{
	DepthBuffer = new float[ScreenSize.X * ScreenSize.Y];
}

void WindowsGDI::ClearDepthBuffer()
{
	if (DepthBuffer != nullptr)
	{
		float* current = DepthBuffer;
		float defValue = INFINITY;
		unsigned long totalCount = ScreenSize.X * ScreenSize.Y;
		while (totalCount--)
		{
			*current++ = defValue;
		}
	}
}

float WindowsGDI::GetDepthBufferValue(const ScreenPoint& InPos) const
{
	if (DepthBuffer == nullptr)
	{
		return INFINITY;
	}

	if (!IsInScreen(InPos))
	{
		return INFINITY;
	}

	ScreenPoint halfSize = ScreenSize.GetHalfSize();
	long offset = ScreenSize.X * halfSize.Y + halfSize.X + InPos.X + ScreenSize.X * -InPos.Y;
	return *(DepthBuffer + offset);
}

void WindowsGDI::SetDepthBufferValue(const ScreenPoint& InPos, float InDepthValue)
{
	if (DepthBuffer == nullptr)
	{
		return;
	}

	if (!IsInScreen(InPos))
	{
		return;
	}

	ScreenPoint halfSize = ScreenSize.GetHalfSize();
	long offset = ScreenSize.X * halfSize.Y + halfSize.X + InPos.X + ScreenSize.X * -InPos.Y;
	*(DepthBuffer + offset) = InDepthValue;
}