
#include "Precompiled.h"
#include "Transform2D.h"

Matrix3x3 Transform2D::GetTRS() const
{
	float rad = Math::Deg2Rad(RotationInDegree);
	Matrix3x3 tMat = Matrix3x3(Vector3::UnitX, Vector3::UnitY, Vector3(Position.X, Position.Y, 1.f));
	Matrix3x3 rMat = Matrix3x3(Vector3(cosf(rad), sinf(rad), 0.f), Vector3(-sinf(rad), cosf(rad), 0.f), Vector3::UnitZ);
	Matrix3x3 sMat = Matrix3x3(Vector3::UnitX * Scale.X, Vector3::UnitY * Scale.Y, Vector3::UnitZ);

	return tMat * rMat * sMat;
}

void Transform2D::SetPosition(const Vector2 & InPosition)
{
	Position = InPosition;
}

void Transform2D::SetRotation(float InEulerRotationInDegree)
{
	RotationInDegree = InEulerRotationInDegree;
}

void Transform2D::SetScale(const Vector2 & InScale)
{
	Scale = InScale;
}

void Transform2D::AddPosition(const Vector2 & InPosition)
{
	Position += InPosition;
}

void Transform2D::AddRotation(float InRotationDegree)
{
	RotationInDegree += InRotationDegree;
}

void Transform2D::AddScale(const Vector2 & InScale)
{
	Scale += InScale;
}

