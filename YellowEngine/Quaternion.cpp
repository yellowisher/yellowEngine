#include <cmath>

#include "Utils.hpp"
#include "Quaternion.hpp"


const Quaternion Quaternion::identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
const Quaternion Quaternion::zero = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);


Quaternion::Quaternion() :x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}


Quaternion::Quaternion(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{
}


Quaternion::Quaternion(const Quaternion& quaternion)
{
	*this = quaternion;
}


Quaternion::Quaternion(const Vector3& eulerAngle)
{
	float cy = cosf(Utils::rad2deg(eulerAngle.x * 0.5f));
	float sy = sinf(Utils::rad2deg(eulerAngle.x * 0.5f));
	float cp = cosf(Utils::rad2deg(eulerAngle.y * 0.5f));
	float sp = sinf(Utils::rad2deg(eulerAngle.y * 0.5f));
	float cr = cosf(Utils::rad2deg(eulerAngle.z * 0.5f));
	float sr = sinf(Utils::rad2deg(eulerAngle.z * 0.5f));

	w = cy * cp * cr + sy * sp * sr;
	x = cy * cp * sr - sy * sp * cr;
	y = sy * cp * sr + cy * sp * cr;
	z = sy * cp * cr - cy * sp * sr;
}


Quaternion::Quaternion(const Matrix& matrix)
{
	Vector3 xaxis(matrix.m[0], matrix.m[1], matrix.m[2]);
	Vector3 yaxis(matrix.m[4], matrix.m[5], matrix.m[6]);
	Vector3 zaxis(matrix.m[8], matrix.m[9], matrix.m[10]);

	float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

	if (trace > 1.0f)
	{
		float s = 0.5f / sqrt(trace);
		w = 0.25f / s;
		x = (yaxis.z - zaxis.y) * s;
		y = (zaxis.x - xaxis.z) * s;
		z = (xaxis.y - yaxis.x) * s;
	}
	else
	{
		if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
		{
			float s = 0.5f / sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
			w = (yaxis.z - zaxis.y) * s;
			x = 0.25f / s;
			y = (yaxis.x + xaxis.y) * s;
			z = (zaxis.x + xaxis.z) * s;
		}
		else if (yaxis.y > zaxis.z)
		{
			float s = 0.5f / sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
			w = (zaxis.x - xaxis.z) * s;
			x = (yaxis.x + xaxis.y) * s;
			y = 0.25f / s;
			z = (zaxis.y + yaxis.z) * s;
		}
		else
		{
			float s = 0.5f / sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y);
			w = (xaxis.y - yaxis.x) * s;
			x = (zaxis.x + xaxis.z) * s;
			y = (zaxis.y + yaxis.z) * s;
			z = 0.25f / s;
		}
	}
}


Quaternion::~Quaternion()
{
}


Quaternion& Quaternion::operator=(const Quaternion& quaternion)
{
	x = quaternion.x;
	y = quaternion.y;
	z = quaternion.z;
	w = quaternion.w;
	return *this;
}


Quaternion Quaternion::operator~() const
{
	return this->conjugate();
}


// Hamilton product
Quaternion Quaternion::operator*(const Quaternion& quaternion) const
{
	const Quaternion& q = quaternion;
	float nx = w * q.x + x * q.w + y * q.z - z * q.y;
	float ny = w * q.y - x * q.z + y * q.w + z * q.x;
	float nz = w * q.z + x * q.y - y * q.x + z * q.w;
	float nw = w * q.w - x * q.x - y * q.y - z * q.z;

	return Quaternion(nx, ny, nz, nw);
}


Quaternion& Quaternion::operator*=(const Quaternion& quaternion)
{
	*this = *this * quaternion;
	return *this;
}


Vector3 Quaternion::operator*(const Vector3& point) const
{
	Quaternion p(point.x, point.y, point.z, 0);
	p = *this * p;
	p *= ~(*this);
	return Vector3(p.x, p.y, p.z);
}


Quaternion Quaternion::conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}


Vector3 Quaternion::toEulerAngle()
{
	normalize();
	Vector3 rotation;
	float sinr_cosp = 2.0f * (w * x + y * z);
	float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
	rotation.z = atan2(sinr_cosp, cosr_cosp);

	float sinp = 2.0f * (w * y - z * x);
	if (fabs(sinp) >= 1.0f) rotation.y = copysignf(Utils::pi / 2.0f, sinp);
	else rotation.y = asin(sinp);

	float siny_cosp = 2.0f * (w * z + x * y);
	float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
	rotation.x = atan2(siny_cosp, cosy_cosp);

	return rotation;
}


Matrix Quaternion::toMatrix()
{
	normalize();
	return Matrix(
		1.0f - 2.0f*y*y - 2.0f*z*z, 2.0f*x*y - 2.0f*z*w, 2.0f*x*z + 2.0f*y*w, 0.0f,
		2.0f*x*y + 2.0f*z*w, 1.0f - 2.0f*x*x - 2.0f*z*z, 2.0f*y*z - 2.0f*x*w, 0.0f,
		2.0f*x*z - 2.0f*y*w, 2.0f*y*z + 2.0f*x*w, 1.0f - 2.0f*x*x - 2.0f*y*y, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}


void Quaternion::normalize()
{
	float m = x * x + y * y + z * z + w * w;
	if (fabs(m - 1.0f) < Utils::epsilon || fabs(m) < Utils::epsilon)return;

	m = sqrt(m);
	m = 1.0f / m;
	x *= m;
	y *= m;
	z *= m;
	w *= m;
}


Quaternion Quaternion::lerp(Quaternion& q0, Quaternion& q1, float delta)
{
	return identity;
}