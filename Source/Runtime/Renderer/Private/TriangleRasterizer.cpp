
#include "Precompiled.h"
#include "TriangleRasterizer.h"

TriangleRasterizer::TriangleRasterizer()
{
	RecalcBounds();
}

TriangleRasterizer::TriangleRasterizer(VertexData InVertex0, VertexData InVertex1, VertexData InVertex2)
{
	VertexBuffer[0] = InVertex0;
	VertexBuffer[1] = InVertex1;
	VertexBuffer[2] = InVertex2;

	bHasColor = true;
	bHasUV = true;
		
	RecalcBounds();
}

void TriangleRasterizer::RecalcBounds()
{
	// screen bounds
	Vector2 sbbMin2D = Vector2(INFINITY, INFINITY);
	Vector2 sbbMax2D = Vector2(-INFINITY, -INFINITY);

	for (int i = 0; i < 3; i++)
	{
		if (VertexBuffer[i].Position.X < sbbMin2D.X) sbbMin2D.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y < sbbMin2D.Y) sbbMin2D.Y = VertexBuffer[i].Position.Y;
		if (VertexBuffer[i].Position.X > sbbMax2D.X) sbbMax2D.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y > sbbMax2D.Y) sbbMax2D.Y = VertexBuffer[i].Position.Y;
	}

	UVector = VertexBuffer[1].Position - VertexBuffer[0].Position;
	VVector = VertexBuffer[2].Position - VertexBuffer[0].Position;
	DotUU = UVector.Dot(UVector);
	DotUV = UVector.Dot(VVector);
	DotVV = VVector.Dot(VVector);
	InvDenom = 1.f / (DotUU * DotVV - DotUV * DotUV);

	TopLeft = ScreenPoint(sbbMin2D);
	BottomRight = ScreenPoint(sbbMax2D);
}

