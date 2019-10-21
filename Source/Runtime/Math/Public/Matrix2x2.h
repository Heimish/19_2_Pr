#pragma once

#include "Vector2.h"

struct Matrix2x2
{
public:
	Matrix2x2();
	Matrix2x2(const Vector2& InCol0, const Vector2& InCol1);
	Matrix2x2(float In00, float In01, float In10, float In11);

	FORCEINLINE void SetIdentity();
	FORCEINLINE void SetRotation(float InRadian);
	FORCEINLINE void SetScale(Vector2 InScale);
	FORCEINLINE Matrix2x2 Tranpose() const;

	FORCEINLINE const Vector2& operator[](int InIndex) const;
	FORCEINLINE Vector2& operator[](int InIndex);

	FORCEINLINE Matrix2x2 operator*(const Matrix2x2& InM) const;

	FORCEINLINE Vector2 operator*(const Vector2& InV) const;
	FORCEINLINE friend Vector2 operator*=(Vector2& InV, const Matrix2x2& InM)
	{
		InV = InM * InV;
		return InV;
	}

	FORCEINLINE Matrix2x2 operator*(float InS) const;

	static const Matrix2x2 Identity;

private:
	Vector2 Cols[2];
};

FORCEINLINE void Matrix2x2::SetIdentity()
{
	*this = Matrix2x2::Identity;
}

FORCEINLINE void Matrix2x2::SetRotation(float InRadian)
{
	Cols[0] = Vector2(cosf(InRadian), sinf(InRadian));
	Cols[1] = Vector2(-sinf(InRadian), cosf(InRadian));
}

FORCEINLINE void Matrix2x2::SetScale(Vector2 InScale)
{
	Cols[0] = Vector2::UnitX * InScale.X;
	Cols[1] = Vector2::UnitY * InScale.Y;
}

FORCEINLINE Matrix2x2 Matrix2x2::Tranpose() const
{
	return Matrix2x2(
		Vector2(Cols[0].X, Cols[1].X),
		Vector2(Cols[0].Y, Cols[1].Y)
	);
}


FORCEINLINE const Vector2& Matrix2x2::operator[](int InIndex) const
{
	return Cols[InIndex];
}

FORCEINLINE Vector2& Matrix2x2::operator[](int InIndex)
{
	return Cols[InIndex];
}



FORCEINLINE Matrix2x2 Matrix2x2::operator*(const Matrix2x2 &InM) const
{
	Matrix2x2 tpMat = Tranpose();
	return Matrix2x2(
		Vector2(tpMat[0].Dot(InM[0]), tpMat[1].Dot(InM[0])),
		Vector2(tpMat[0].Dot(InM[1]), tpMat[1].Dot(InM[1]))
	);
}

FORCEINLINE Vector2 Matrix2x2::operator*(const Vector2& InV) const
{
	Matrix2x2 tpMat = Tranpose();
	return Vector2(
		tpMat[0].Dot(InV),
		tpMat[1].Dot(InV)
	);
}

FORCEINLINE Matrix2x2 Matrix2x2::operator*(float InS) const
{
	return Matrix2x2(
		Cols[0] * InS,
		Cols[1] * InS
	);
}
