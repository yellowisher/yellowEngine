#ifndef __H_MATRIX__
#define __H_MATRIX__

#include "Vector3.hpp"
#include "Vector4.hpp"

class Quaternion;

class Matrix
{
public:
	static const Matrix zero;
	static const Matrix identity;

	// should be in MatrixUtils?
	static Matrix createPerspective(float fov, float aspectRatio, float zNear, float zFar);
	static Matrix createOrthographic(float width, float height, float zNear, float zFar);
	static Matrix createTranslation(Vector3 translation);
	static Matrix createRotation(Quaternion rotation);
	static Matrix createScale(Vector3 scale);
	static Vector3 extractTranslation(const Matrix& tr);
	static Quaternion extractRotation(const Matrix& tr);
	static Vector3 extractScale(const Matrix& s);

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
	Matrix operator~() const;

	Matrix& operator=(const Matrix& matrix);
	Matrix& operator+=(const Matrix& matrix);
	Matrix& operator-=(const Matrix& matrix);
	Matrix& operator*=(const Matrix& matrix);

	Vector3 operator*(const Vector3& vector) const;
	Vector3& operator*=(Vector3& vector) const;
	Vector4 operator*(const Vector4& vector) const;
	Vector4& operator*=(Vector4& vector) const;

private:
	static const int MATRIX_SIZE;

	union
	{
		float _m[16];
		struct
		{
			float _m00; float _m10; float _m20; float _m30;
			float _m01; float _m11; float _m21; float _m31;
			float _m02; float _m12; float _m22; float _m32;
			float _m03; float _m13; float _m23; float _m33;
		};
	};
};

#endif