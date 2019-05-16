#define DEG_TO_RAD(x) ((x) * 0.0174532925f)
#define M_PI 3.14159265358979323846
#define M_EPSILON 0.000001f

#include <iostream>
#include <cstring>
#include <cmath>

#include "Matrix.hpp"


const Matrix Matrix::zero(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);

const Matrix Matrix::identity(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

const int Matrix::MATRIX_SIZE = sizeof(_m);


Matrix Matrix::createPerspective(float fieldOfView, float aspectRatio, float zNear, float zFar)
{
	Matrix matrix = Matrix::zero;

	float f_n = 1.0f / (zFar - zNear);
	float theta = DEG_TO_RAD(fieldOfView) * 0.5f;
	if (fabs(fmod(theta, M_PI*0.5f)) < M_EPSILON)
	{
		std::cout << "Invalid operation in createPerspective()" << std::endl;
		return matrix;
	}
	float divisor = tan(theta);
	float factor = 1.0f / divisor;

	matrix._m[0] = (1.0f / aspectRatio) * factor;
	matrix._m[5] = factor;
	matrix._m[10] = (zFar + zNear) * -f_n;
	matrix._m[11] = -1.0f;
	matrix._m[14] = -2.0f * zFar * zNear * f_n;

	return matrix;
}


Matrix Matrix::createOrthographic(float width, float height, float zNear, float zFar)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	float left = -halfWidth;
	float right = halfWidth;
	float top = halfHeight;
	float bottom = -halfHeight;

	Matrix matrix = Matrix::zero;
	matrix._m[0] = 2.0f / (right - left);
	matrix._m[5] = 2.0f / (top - bottom);
	matrix._m[10] = 2.0f / (zNear - zFar);
	matrix._m[12] = (left + right) / (left - right);
	matrix._m[13] = (top + bottom) / (bottom - top);
	matrix._m[14] = zNear / (zNear - zFar);
	matrix._m[15] = 1.0f;

	return matrix;
}


Matrix::Matrix()
{
	*this = identity;
}


Matrix::Matrix(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
)
{
	_m[0] = m00; _m[1] = m01; _m[2] = m02; _m[3] = m03;
	_m[4] = m10; _m[5] = m11; _m[6] = m12; _m[7] = m13;
	_m[8] = m20; _m[9] = m21; _m[10] = m22; _m[11] = m23;
	_m[12] = m30; _m[13] = m31; _m[14] = m32; _m[15] = m33;
}


Matrix::Matrix(const Matrix& matrix)
{
	std::memcpy(_m, matrix._m, MATRIX_SIZE);
}


Matrix::~Matrix()
{

}


Matrix Matrix::operator+(const Matrix& matrix) const
{
	return Matrix(
		_m[0] + matrix._m[0], _m[1] + matrix._m[1], _m[2] + matrix._m[2], _m[3] + matrix._m[3],
		_m[4] + matrix._m[4], _m[5] + matrix._m[5], _m[6] + matrix._m[6], _m[7] + matrix._m[7],
		_m[8] + matrix._m[8], _m[9] + matrix._m[9], _m[10] + matrix._m[10], _m[11] + matrix._m[11],
		_m[12] + matrix._m[12], _m[13] + matrix._m[13], _m[14] + matrix._m[14], _m[15] + matrix._m[15]
	);
}


Matrix Matrix::operator-(const Matrix& matrix) const
{
	return Matrix(
		_m[0] - matrix._m[0], _m[1] - matrix._m[1], _m[2] - matrix._m[2], _m[3] - matrix._m[3],
		_m[4] - matrix._m[4], _m[5] - matrix._m[5], _m[6] - matrix._m[6], _m[7] - matrix._m[7],
		_m[8] - matrix._m[8], _m[9] - matrix._m[9], _m[10] - matrix._m[10], _m[11] - matrix._m[11],
		_m[12] - matrix._m[12], _m[13] - matrix._m[13], _m[14] - matrix._m[14], _m[15] - matrix._m[15]
	);
}


Matrix Matrix::operator*(const Matrix& matrix) const
{
	const float* m0 = _m;
	const float* m1 = matrix._m;

	Matrix result;
	result._m[0] = m0[0] * m1[0] + m0[1] * m1[4] + m0[2] * m1[8] + m0[3] * m1[12];
	result._m[1] = m0[0] * m1[1] + m0[1] * m1[5] + m0[2] * m1[9] + m0[3] * m1[13];
	result._m[2] = m0[0] * m1[2] + m0[1] * m1[6] + m0[2] * m1[10] + m0[3] * m1[14];
	result._m[3] = m0[0] * m1[3] + m0[1] * m1[7] + m0[2] * m1[11] + m0[3] * m1[15];

	result._m[4] = m0[4] * m1[0] + m0[5] * m1[4] + m0[6] * m1[8] + m0[7] * m1[12];
	result._m[5] = m0[4] * m1[1] + m0[5] * m1[5] + m0[6] * m1[9] + m0[7] * m1[13];
	result._m[6] = m0[4] * m1[2] + m0[5] * m1[6] + m0[6] * m1[10] + m0[7] * m1[14];
	result._m[7] = m0[4] * m1[3] + m0[5] * m1[7] + m0[6] * m1[11] + m0[7] * m1[15];

	result._m[8] = m0[8] * m1[0] + m0[9] * m1[4] + m0[10] * m1[8] + m0[11] * m1[12];
	result._m[9] = m0[8] * m1[1] + m0[9] * m1[5] + m0[10] * m1[9] + m0[11] * m1[13];
	result._m[10] = m0[8] * m1[2] + m0[9] * m1[6] + m0[10] * m1[10] + m0[11] * m1[14];
	result._m[11] = m0[8] * m1[3] + m0[9] * m1[7] + m0[10] * m1[11] + m0[11] * m1[15];

	result._m[12] = m0[12] * m1[0] + m0[13] * m1[4] + m0[14] * m1[8] + m0[15] * m1[12];
	result._m[13] = m0[12] * m1[1] + m0[13] * m1[5] + m0[14] * m1[9] + m0[15] * m1[13];
	result._m[14] = m0[12] * m1[2] + m0[13] * m1[6] + m0[14] * m1[10] + m0[15] * m1[14];
	result._m[15] = m0[12] * m1[3] + m0[13] * m1[7] + m0[14] * m1[11] + m0[15] * m1[15];

	return result;
}


Matrix& Matrix::operator=(const Matrix& matrix)
{
	memcpy(_m, matrix._m, MATRIX_SIZE);
	return *this;
}


Matrix& Matrix::operator+=(const Matrix& matrix)
{
	*this = *this + matrix;
	return *this;
}


Matrix& Matrix::operator-=(const Matrix& matrix)
{
	*this = *this - matrix;
	return *this;
}


Matrix& Matrix::operator*=(const Matrix& matrix)
{
	*this = *this * matrix;
	return *this;
}


Vector4 Matrix::operator*(const Vector4& vector) const
{
	const float* v = vector.v;

	Vector4 result;
	result.v[0] = _m[0] * v[0] + _m[1] * v[1] + _m[2] * v[2] + _m[3] * v[3];
	result.v[1] = _m[4] * v[0] + _m[5] * v[1] + _m[6] * v[2] + _m[7] * v[3];
	result.v[2] = _m[8] * v[0] + _m[9] * v[1] + _m[10] * v[2] + _m[11] * v[3];
	result.v[3] = _m[12] * v[0] + _m[13] * v[1] + _m[14] * v[2] + _m[15] * v[3];

	return result;
}


Vector4& Matrix::operator*=(Vector4& vector) const
{
	vector = *this * vector;
	return vector;
}