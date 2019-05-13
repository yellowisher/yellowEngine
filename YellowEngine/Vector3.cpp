#include <cmath>

#include "Vector3.hpp"

Vector3::Vector3()
{

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