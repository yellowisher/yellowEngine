#ifndef __H_VECTOR2__
#define __H_VECTOR2__

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& vector);
	~Vector2();

	Vector2 operator+(const Vector2& vector) const;
	Vector2 operator-(const Vector2& vector) const;
	Vector2 operator*(float value) const;
	Vector2 operator/(float value) const;
	bool operator<(const Vector2& vector) const;
	bool operator==(const Vector2& vector) const;
	bool operator!=(const Vector2& vector) const;

	void normalize();
	float magnitude();
};

#endif