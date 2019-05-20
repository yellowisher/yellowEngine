#include <cmath>

#include "Vector2.hpp"

Vector2::Vector2()
{
	set(0, 0);
}


Vector2::Vector2(float x, float y) :x(x), y(y)
{

}


Vector2::Vector2(const Vector2& vector) :x(vector.x), y(vector.y)
{

}


Vector2::~Vector2()
{

}


void Vector2::set(float x, float y)
{
	this->x = x;
	this->y = y;
}


Vector2 Vector2::operator+(const Vector2& vector) const
{
	Vector2 result(*this);
	result.x += vector.x;
	result.y += vector.y;
	return result;
}


Vector2 Vector2::operator-(const Vector2& vector) const
{
	Vector2 result(*this);
	result.x -= vector.x;
	result.y -= vector.y;
	return result;
}


Vector2 Vector2::operator*(float value) const
{
	Vector2 result(*this);
	result.x *= value;
	result.y *= value;
	return result;
}


Vector2 Vector2::operator/(float value) const
{
	Vector2 result(*this);
	result.x /= value;
	result.y /= value;
	return result;
}


Vector2& Vector2::operator= (const Vector2& vector)
{
	set(vector.x, vector.y);
	return *this;
}


Vector2& Vector2::operator+= (const Vector2& vector)
{
	*this = *this + vector;
	return *this;
}


Vector2& Vector2::operator-= (const Vector2& vector)
{
	*this = *this - vector;
	return *this;
}


bool Vector2::operator<(const Vector2& vector) const
{
	if (x == vector.x)
	{
		return y < vector.y;
	}
	return x < vector.x;
}


bool Vector2::operator==(const Vector2& vector) const
{
	return (x == vector.x) && (y == vector.y);
}


bool Vector2::operator!=(const Vector2& vector) const
{
	return !(*this == vector);
}


void Vector2::normalize()
{
	float mag = magnitude();
	if (mag == 1.0f || mag == 0.0f)return;

	float div = sqrt(mag);
	*this = *this / div;
}


float Vector2::magnitude()
{
	return x * x + y * y;
}