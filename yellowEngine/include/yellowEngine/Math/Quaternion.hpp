#ifndef __H_QUATERNION__
#define __H_QUATERNION__

#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Math/Vector4.hpp"
#include "yellowEngine/Math/Matrix.hpp"

namespace yellowEngine
{
	class Quaternion
	{
	public:
		static const Quaternion identity;
		static const Quaternion zero;

		static Quaternion lerp(Quaternion q0, Quaternion q1, float delta);
		static Quaternion axisAngle(Vector3 axis, float rad);

		float x;
		float y;
		float z;
		float w;

		Quaternion();
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Quaternion& quaternion);
		Quaternion(Vector3 eulerAngle, bool rad = false);
		//Quaternion(const Matrix& matrix);
		~Quaternion();

		Quaternion operator~() const;
		Quaternion& operator=(const Quaternion& quaternion);
		Quaternion operator*(const Quaternion& quaternion) const;
		Quaternion& operator*=(const Quaternion& quaternion);
		Vector3 operator*(const Vector3& point) const;

		bool operator==(const Quaternion& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator!=(const Quaternion& other) const { return !(*this == other); }

		Quaternion conjugate() const;
		Vector3 toEulerAngle() const;
		Matrix toMatrix();
		Quaternion& normalize();
	};
}

#endif