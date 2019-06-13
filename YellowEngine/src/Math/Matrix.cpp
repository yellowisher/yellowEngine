#include <iostream>
#include <cstring>
#include <cmath>

#include "yellowEngine/Math/Matrix.hpp"
#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Math/Quaternion.hpp"


namespace yellowEngine
{
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


	Matrix Matrix::createPerspective(float fov, float aspectRatio, float zNear, float zFar)
	{
		Matrix matrix = Matrix::zero;

		float n_f = 1.0f / (zNear - zFar);
		float theta = Utils::deg2rad(fov) * 0.5f;

		if (fabs(fmod(theta, Utils::pi*0.5f)) < Utils::epsilon)
		{
			std::cout << "Invalid operation in createPerspective()" << std::endl;
			return matrix;
		}
		float factor = 1.0f / tan(theta);

		matrix._m[0] = (factor / aspectRatio);
		matrix._m[5] = factor;
		matrix._m[10] = (zFar + zNear) * n_f;
		matrix._m[11] = -1.0f;
		matrix._m[14] = 2.0f * zFar * zNear * n_f;

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

		Matrix matrix = Matrix::identity;
		matrix._m[0] = 2.0f / (right - left);
		matrix._m[5] = 2.0f / (top - bottom);
		matrix._m[10] = 2.0f / (zNear - zFar);
		matrix._m[12] = (left + right) / (left - right);
		matrix._m[13] = (top + bottom) / (bottom - top);
		matrix._m[14] = (zFar + zNear) / (zNear - zFar);

		return matrix;
	}


	Matrix Matrix::createTranslation(Vector3 translation)
	{
		Matrix t = Matrix::identity;
		t._m[12] = translation.x;
		t._m[13] = translation.y;
		t._m[14] = translation.z;
		return t;
	}


	Matrix Matrix::createRotation(Quaternion rotation)
	{
		return rotation.toMatrix();
	}


	Matrix Matrix::createScale(Vector3 scale)
	{
		Matrix s = Matrix::identity;
		s._m00 = scale.x;
		s._m11 = scale.y;
		s._m22 = scale.z;
		return s;
	}


	Vector3 Matrix::extractTranslation() const
	{
		return Vector3(_m[12], _m[13], _m[14]);
	}


	Quaternion Matrix::extractRotation() const
	{
		return Quaternion(*this);
	}


	Vector3 Matrix::extractScale() const
	{
		return Vector3(_m[0], _m[5], _m[10]);
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
		_m[0] = m00; _m[1] = m10; _m[2] = m20; _m[3] = m30;
		_m[4] = m01; _m[5] = m11; _m[6] = m21; _m[7] = m31;
		_m[8] = m02; _m[9] = m12; _m[10] = m22; _m[11] = m32;
		_m[12] = m03; _m[13] = m13; _m[14] = m23; _m[15] = m33;
	}


	Matrix::Matrix(const Matrix& matrix)
	{
		std::memcpy(_m, matrix._m, MATRIX_SIZE);
	}


	Matrix::~Matrix()
	{

	}


	float& Matrix::operator[](int i)
	{
		return _m[i];
	}

	float Matrix::operator[](int i) const
	{
		return _m[i];
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

		result._m[0] = m0[0] * m1[0] + m0[4] * m1[1] + m0[8] * m1[2] + m0[12] * m1[3];
		result._m[1] = m0[1] * m1[0] + m0[5] * m1[1] + m0[9] * m1[2] + m0[13] * m1[3];
		result._m[2] = m0[2] * m1[0] + m0[6] * m1[1] + m0[10] * m1[2] + m0[14] * m1[3];
		result._m[3] = m0[3] * m1[0] + m0[7] * m1[1] + m0[11] * m1[2] + m0[15] * m1[3];

		result._m[4] = m0[0] * m1[4] + m0[4] * m1[5] + m0[8] * m1[6] + m0[12] * m1[7];
		result._m[5] = m0[1] * m1[4] + m0[5] * m1[5] + m0[9] * m1[6] + m0[13] * m1[7];
		result._m[6] = m0[2] * m1[4] + m0[6] * m1[5] + m0[10] * m1[6] + m0[14] * m1[7];
		result._m[7] = m0[3] * m1[4] + m0[7] * m1[5] + m0[11] * m1[6] + m0[15] * m1[7];

		result._m[8] = m0[0] * m1[8] + m0[4] * m1[9] + m0[8] * m1[10] + m0[12] * m1[11];
		result._m[9] = m0[1] * m1[8] + m0[5] * m1[9] + m0[9] * m1[10] + m0[13] * m1[11];
		result._m[10] = m0[2] * m1[8] + m0[6] * m1[9] + m0[10] * m1[10] + m0[14] * m1[11];
		result._m[11] = m0[3] * m1[8] + m0[7] * m1[9] + m0[11] * m1[10] + m0[15] * m1[11];

		result._m[12] = m0[0] * m1[12] + m0[4] * m1[13] + m0[8] * m1[14] + m0[12] * m1[15];
		result._m[13] = m0[1] * m1[12] + m0[5] * m1[13] + m0[9] * m1[14] + m0[13] * m1[15];
		result._m[14] = m0[2] * m1[12] + m0[6] * m1[13] + m0[10] * m1[14] + m0[14] * m1[15];
		result._m[15] = m0[3] * m1[12] + m0[7] * m1[13] + m0[11] * m1[14] + m0[15] * m1[15];

		return result;
	}


	Matrix Matrix::operator~() const
	{
		float m3[3][3];
		float det =
			_m00 * (_m11 * _m22 - _m21 * _m12) -
			_m01 * (_m10 * _m22 - _m12 * _m20) +
			_m02 * (_m10 * _m21 - _m11 * _m20);

		if (det == 0)
		{
			std::cout << "Wrong operation in invering Matrix" << std::endl;
			return Matrix::identity;
		}

		det = 1.0f / det;
		m3[0][0] = (_m11 * _m22 - _m21 * _m12) * det;
		m3[0][1] = (_m02 * _m21 - _m01 * _m22) * det;
		m3[0][2] = (_m01 * _m12 - _m02 * _m11) * det;
		m3[1][0] = (_m12 * _m20 - _m10 * _m22) * det;
		m3[1][1] = (_m00 * _m22 - _m02 * _m20) * det;
		m3[1][2] = (_m10 * _m02 - _m00 * _m12) * det;
		m3[2][0] = (_m10 * _m21 - _m20 * _m11) * det;
		m3[2][1] = (_m20 * _m01 - _m00 * _m21) * det;
		m3[2][2] = (_m00 * _m11 - _m10 * _m01) * det;


		return Matrix(
			m3[0][0], m3[0][1], m3[0][2], -m3[0][0] * _m[12] - m3[0][1] * _m[13] - m3[0][2] * _m[14],
			m3[1][0], m3[1][1], m3[1][2], -m3[1][0] * _m[12] - m3[1][1] * _m[13] - m3[1][2] * _m[14],
			m3[2][0], m3[2][1], m3[2][2], -m3[2][0] * _m[12] - m3[2][1] * _m[13] - m3[2][2] * _m[14],
			0, 0, 0, 1.0f
		);
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


	Vector3 Matrix::operator*(const Vector3& vector) const
	{
		Vector4 result = *this * Vector4(vector.x, vector.y, vector.z, 1.0f);
		return Vector3(result.x, result.y, result.z);
	}


	Vector3& Matrix::operator*=(Vector3& vector) const
	{
		Vector4 result = *this * Vector4(vector.x, vector.y, vector.z, 1.0f);
		vector.set(result.x, result.y, result.z);
		return vector;
	}


	Vector4 Matrix::operator*(const Vector4& vector) const
	{
		const float* v = vector.v;

		Vector4 result;
		result.v[0] = _m00 * v[0] + _m01 * v[1] + _m02 * v[2] + _m03 * v[3];
		result.v[1] = _m10 * v[0] + _m11 * v[1] + _m12 * v[2] + _m13 * v[3];
		result.v[2] = _m20 * v[0] + _m21 * v[1] + _m22 * v[2] + _m23 * v[3];
		result.v[3] = _m30 * v[0] + _m31 * v[1] + _m32 * v[2] + _m33 * v[3];

		return result;
	}


	Vector4& Matrix::operator*=(Vector4& vector) const
	{
		vector = *this * vector;
		return vector;
	}
}