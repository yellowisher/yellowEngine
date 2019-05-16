#ifndef __H_MATRIX__
#define __H_MATRIX__

#include "Vector4.hpp"

class Matrix
{
public:
	static const Matrix zero;
	static const Matrix identity;

	static Matrix createPerspective(float fieldOfView, float aspectRatio, float zNear, float zFar);
	static Matrix createOrthographic(float width, float height, float zNear, float zFar);

	const float* const m = _m;

	Matrix();
	Matrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	);
	Matrix(const Matrix& matrix);
	~Matrix();

	Matrix operator+(const Matrix& matrix) const;
	Matrix operator-(const Matrix& matrix) const;
	Matrix operator*(const Matrix& matrix) const;

	Matrix& operator=(const Matrix& matrix);
	Matrix& operator+=(const Matrix& matrix);
	Matrix& operator-=(const Matrix& matrix);
	Matrix& operator*=(const Matrix& matrix);

	Vector4 operator*(const Vector4& vector) const;
	Vector4& operator*=(Vector4& vector) const;

private:
	static const int MATRIX_SIZE;

	union
	{
		float _m[16];
		float _m2[4][4];
	};
};

#endif