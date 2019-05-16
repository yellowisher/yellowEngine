#ifndef __H_QUATERNION__
#define __H_QUATERNION__

#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"

class Quaternion
{
public:
	static const Quaternion identity;
	static const Quaternion zero;

	float x;
	float y;
	float z;
	float w;

	static Quaternion lerp(Quaternion& q0, Quaternion& q1, float delta);

	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Quaternion& quaternion);
	Quaternion(const Vector3& eulerAngle);
	Quaternion(const Matrix& matrix);
	~Quaternion();

	Quaternion& operator=(const Quaternion& quaternion);
	Quaternion operator*(const Quaternion& quaternion) const;
	Quaternion& operator*=(const Quaternion& quaternion);

	Vector3 toEulerAngle();
	Matrix toMatrix();
	void normalize();


private:
};

#endif