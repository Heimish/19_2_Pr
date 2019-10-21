
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"

void SoftRenderer::Initialize()
{
	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);
	}

	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
	{
		return;
	}
	CyclesPerMilliSeconds = (float)(frequency.QuadPart / 1000.f);
}

void SoftRenderer::Shutdown()
{
	if (RSI != nullptr && RSI->IsIntialized())
	{
		RSI->Shutdown();
		delete RSI;
		RSI = nullptr;
	}
}

void SoftRenderer::Update()
{
	if (RSI != nullptr)
	{
		// Render Start
		RSI->Clear(LinearColor(0.125f, 0.5f, 1.f, 1.f));


		// X.Axis
		RSI->DrawVerticalLine(0, LinearColor(0.f, 1.f, 0.f, 1.f));
		RSI->DrawHorizontalLine(0, LinearColor(1.f, 0.f, 0.f, 1.f));

		// Grid Line
		ScreenPoint screenSize = DisplaySetting::Inst().GetSize();
		static int gridSize = 10;
		static LinearColor gridColor = LinearColor(0.5f, 0.5f, 0.5f, 1.f);
		int halfX = Math::FloorToInt(((float)screenSize.X - 1.f) * 0.5f);
		int halfY = Math::FloorToInt(((float)screenSize.Y - 1.f) * 0.5f);
		for (int ix = gridSize; ix < halfX; ix += gridSize)
		{
			RSI->DrawVerticalLine(ix, gridColor);
			RSI->DrawVerticalLine(-ix, gridColor);
		}

		for (int iy = gridSize; iy < halfY; iy += gridSize)
		{
			RSI->DrawHorizontalLine(iy, gridColor);
			RSI->DrawHorizontalLine(-iy, gridColor);
		}

		float cos, sin = 0.f;
		static float rotateSpeed = 180.0f;
		static float currentAngle = 0.f;
		currentAngle += (FrameTime / 1000.f) * rotateSpeed;
		//float TargetAngle = Math::Deg2Rad(ElapsedTime * rotateSpeed); 
		Math::GetSinCos(cos, sin, currentAngle);
		Matrix2x2 rMat(Vector2(cos, sin), Vector2(-sin, cos));

		float sineWave0To1 = (sinf(ElapsedTime / 1000 * Math::TwoPI) + 1) * 0.5f;
		float minscale = 0.8f;
		float maxscale = 1.6f;
		float currentScale = 0.5f + sineWave0To1 * (maxscale - minscale);
		Matrix2x2 sMat(Vector2::UnitX * currentScale, Vector2::UnitY * currentScale);

		Matrix2x2 RSMat = rMat * sMat; 

		// Define Mesh Data
		VertexData v[4];
		v[0].Position = Vector2(-100.0f, -100.0f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].Position = Vector2(-100.0f, 100.0f);
		v[1].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[2].Position = Vector2(100.0f, 100.0f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);
		v[3].Position = Vector2(100.0f, -100.0f);
		v[3].Color = LinearColor(1.0f, 0.0f, 1.0f);

		for (int i = 0; i <= 3; i++)
		{
			//v[i].Position = sMat * v[i].Position;
			v[i].Position = RSMat * v[i].Position;
		}

		int i[6];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 0;
		i[4] = 2;
		i[5] = 3;

		// Draw Call
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);
		RSI->DrawPrimitive(4, 6);

		// Render Finish
		RSI->EndFrame();
	}
}

void SoftRenderer::PreUpdate()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	FrameTimeStamp = currentTime.QuadPart;
	if (FrameCount == 0)
	{
		StartTimeStamp = currentTime.QuadPart;
	}
}

void SoftRenderer::PostUpdate()
{
	FrameCount++;
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	LONGLONG frameCycles = currentTime.QuadPart - FrameTimeStamp;
	LONGLONG elapsedCycles = currentTime.QuadPart - StartTimeStamp;
	FrameTime = frameCycles / CyclesPerMilliSeconds;
	ElapsedTime = elapsedCycles / CyclesPerMilliSeconds;
	FrameFPS = FrameTime == 0.f ? 0.f : 1000.f / FrameTime;
	AverageFPS = ElapsedTime == 0.f ? 0.f : 1000.f / ElapsedTime * FrameCount;
}


