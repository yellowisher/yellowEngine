#include <cmath>

#include "yellowEngine/Math/Vector3.hpp"


namespace yellowEngine
{
	const Vector3& Vector3::zero = Vector3(0, 0, 0);
	const Vector3& Vector3::up = Vector3(0, 1.0f, 0);
	const Vector3& Vector3::right = Vector3(1.0f, 0, 0);
	const Vector3& Vector3::forward = Vector3(0, 0, 1.0f);


	Vector3 Vector3::lerp(const Vector3 & v0, const Vector3 & v1, float delta)
	{
		return v0 * (1.0f - delta) + v1 * delta;
	}


	Vector3::Vector3()
	{
		set(0, 0, 0);
	}


	Vector3::Vector3(float x, float y, float z) :x(x), y(y), z(z)
	{

	}


	Vector3::Vector3(const Vector3& vector) :x(vector.x), y(vector.y), z(vector.z)
	{

	}


	Vector3::~Vector3()
	{

	}


	void Vector3::set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	Vector3 Vector3::operator+(const Vector3& vector) const
	{
		Vector3 result(*this);
		result.x += vector.x;
		result.y += vector.y;
		result.z += vector.z;
		return result;
	}


	Vector3 Vector3::operator-(const Vector3& vector) const
	{
		Vector3 result(*this);
		result.x -= vector.x;
		result.y -= vector.y;
		result.z -= vector.z;
		return result;
	}


	Vector3 Vector3::operator*(float value) const
	{
		Vector3 result(*this);
		result.x *= value;
		result.y *= value;
		result.z *= value;
		return result;
	}


	Vector3 Vector3::operator/(float value) const
	{
		Vector3 result(*this);
		result.x /= value;
		result.y /= value;
		result.z /= value;
		return result;
	}


	Vector3& Vector3::operator= (const Vector3& vector)
	{
		set(vector.x, vector.y, vector.z);
		return *this;
	}


	Vector3& Vector3::operator+= (const Vector3& vector)
	{
		*this = *this + vector;
		return *this;
	}


	Vector3& Vector3::operator-= (const Vector3& vector)
	{
		*this = *this - vector;
		return *this;
	}


	float Vector3::operator*(const Vector3& vector) const
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}


	bool Vector3::operator<(const Vector3& vector) const
	{
		if (x == vector.x)
		{
			if (y == vector.y)
			{
				return z < vector.z;
			}
			return y < vector.y;
		}
		return x < vector.x;
	}


	bool Vector3::operator==(const Vector3& vector) const
	{
		return (x == vector.x) && (y == vector.y) && (z == vector.z);
	}


	bool Vector3::operator!=(const Vector3& vector) const
	{
		return !(*this == vector);
	}


	void Vector3::normalize()
	{
		float mag = magnitude();
		if (mag == 1.0f || mag == 0.0f)return;

		float div = sqrt(mag);
		*this = *this / div;
	}


	float Vector3::magnitude()
	{
		return x * x + y * y + z * z;
	}
}