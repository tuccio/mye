#include <mye/math/Math.h>

using namespace mye::math;

Vector4 SumTest(const Vector4 & v1, const Vector4 & v2)
{
	return v1 + v2;
}

Real DotTest(const Vector4 & v1, const Vector4 & v2)
{
	return v1.Dot(v2);
}

void CopyTest(Vector4 & dest, const Vector4 & source)
{
	dest = source;
}

Vector4 NormalizeTest(const Vector4 & v)
{
	return v.Normalize();
}

float Determinant3Test(const Matrix3 & m)
{
	return m.Determinant();
}

Matrix3 Inverse3Test(const Matrix3 & m)
{
	return m.Inverse();
}

float Determinant4Test(const Matrix4 & m)
{
	return m.Determinant();
}

Matrix4 Inverse4Test(const Matrix4 & m)
{
	return m.Inverse();
}