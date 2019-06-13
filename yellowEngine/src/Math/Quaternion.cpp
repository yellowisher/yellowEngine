#include <cmath>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Math/Quaternion.hpp"


namespace yellowEngine
{
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
		float cosr = cosf(Utils::deg2rad(eulerAngle.z * 0.5f));
		float sinr = sinf(Utils::deg2rad(eulerAngle.z * 0.5f));
		float cosy = cosf(Utils::deg2rad(eulerAngle.y * 0.5f));
		float siny = sinf(Utils::deg2rad(eulerAngle.y * 0.5f));
		float cosp = cosf(Utils::deg2rad(eulerAngle.x * 0.5f));
		float sinp = sinf(Utils::deg2rad(eulerAngle.x * 0.5f));

		w = cosp * cosy * cosr + sinp * siny * sinr;
		x = sinp * cosy * cosr - cosp * siny * sinr;
		y = cosp * siny * cosr + sinp * cosy * sinr;
		z = cosp * cosy * sinr - sinp * siny * cosr;
	}


	Quaternion::Quaternion(const Matrix& matrix)
	{
		Vector3 xaxis(matrix[0], matrix[1], matrix[2]);
		Vector3 yaxis(matrix[4], matrix[5], matrix[6]);
		Vector3 zaxis(matrix[8], matrix[9], matrix[10]);

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


	Vector3 Quaternion::toEulerAngle() const
	{
		//normalize();
		Vector3 rotation;
		float sinr_cosp = 2.0f * (w * x + y * z);
		float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
		rotation.x = Utils::rad2deg(atan2(sinr_cosp, cosr_cosp));

		float sinp = 2.0f * (w * y - z * x);
		if (fabs(sinp) >= 1.0f) rotation.y = copysignf(Utils::pi / 2.0f, sinp);
		else rotation.y = asin(sinp);
		rotation.y = Utils::rad2deg(rotation.y);
		//rotation.y = Utils::rad2deg(2.0f*(w*y - z * x));

		float siny_cosp = 2.0f * (w * z + x * y);
		float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
		rotation.z = Utils::rad2deg(atan2(siny_cosp, cosy_cosp));

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


	Quaternion& Quaternion::normalize()
	{
		float m = x * x + y * y + z * z + w * w;
		if (fabs(m - 1.0f) < Utils::epsilon || fabs(m) < Utils::epsilon)return *this;

		m = sqrt(m);
		m = 1.0f / m;
		x *= m;
		y *= m;
		z *= m;
		w *= m;

		return *this;
	}


	Quaternion Quaternion::lerp(Quaternion& q0, Quaternion& q1, float delta)
	{
		Quaternion result;
		float delta_ = 1.0f - delta;
		result.x = delta_ * q0.x + delta * q1.x;
		result.y = delta_ * q0.y + delta * q1.y;
		result.z = delta_ * q0.z + delta * q1.z;
		result.w = delta_ * q0.w + delta * q1.w;
		return result;
	}
}