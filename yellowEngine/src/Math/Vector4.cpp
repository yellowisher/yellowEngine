#include <cmath>

#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Math/Vector4.hpp"


namespace yellowEngine
{
	Vector4::Vector4()
	{
		set(0, 0, 0, 0);
	}


	Vector4::Vector4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w)
	{

	}


	Vector4::Vector4(const Vector3& vector, float w) : x(vector.x), y(vector.y), z(vector.z), w(w)
	{
	}


	Vector4::Vector4(const Vector4& vector) :x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{

	}


	Vector4::~Vector4()
	{

	}


	void Vector4::set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}


	Vector4 Vector4::operator+(const Vector4& vector) const
	{
		Vector4 result(*this);
		result.x += vector.x;
		result.y += vector.y;
		result.z += vector.z;
		result.w += vector.w;
		return result;
	}


	Vector4 Vector4::operator-(const Vector4& vector) const
	{
		Vector4 result(*this);
		result.x -= vector.x;
		result.y -= vector.y;
		result.z -= vector.z;
		result.w -= vector.w;
		return result;
	}


	Vector4 Vector4::operator*(float value) const
	{
		Vector4 result(*this);
		result.x *= value;
		result.y *= value;
		result.z *= value;
		result.w *= value;
		return result;
	}


	Vector4 Vector4::operator/(float value) const
	{
		Vector4 result(*this);
		result.x /= value;
		result.y /= value;
		result.z /= value;
		result.w /= value;
		return result;
	}


	Vector4& Vector4::operator= (const Vector3& vector)
	{
		set(vector.x, vector.y, vector.z, 1.0f);
		return *this;
	}


	Vector4& Vector4::operator= (const Vector4& vector)
	{
		set(vector.x, vector.y, vector.z, vector.w);
		return *this;
	}


	Vector4& Vector4::operator+= (const Vector4& vector)
	{
		*this = *this + vector;
		return *this;
	}


	Vector4& Vector4::operator-= (const Vector4& vector)
	{
		*this = *this - vector;
		return *this;
	}


	bool Vector4::operator<(const Vector4& vector) const
	{
		if (x == vector.x)
		{
			if (y == vector.y)
			{
				if (z == vector.z)
				{
					return w < vector.w;
				}
				return z < vector.z;
			}
			return y < vector.y;
		}
		return x < vector.x;
	}


	bool Vector4::operator==(const Vector4& vector) const
	{
		return (x == vector.x) && (y == vector.y) && (z == vector.z) && (w == vector.w);
	}


	bool Vector4::operator!=(const Vector4& vector) const
	{
		return !(*this == vector);
	}


	void Vector4::normalize()
	{
		float mag = magnitude();
		if (mag == 1.0f || mag == 0.0f)return;

		float div = sqrt(mag);
		*this = *this / div;
	}


	float Vector4::magnitude()
	{
		return x * x + y * y + z * z + w * w;
	}
}