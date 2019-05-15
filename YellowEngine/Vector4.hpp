#ifndef __H_VECTOR4__
#define __H_VECTOR4__

class Vector4
{
public:
	union
	{
		float v[4];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};

	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector4& vector);
	~Vector4();

	Vector4 operator+(const Vector4& vector) const;
	Vector4 operator-(const Vector4& vector) const;
	Vector4 operator*(float value) const;
	Vector4 operator/(float value) const;
	bool operator<(const Vector4& vector) const;
	bool operator==(const Vector4& vector) const;
	bool operator!=(const Vector4& vector) const;

	void normalize();
	float magnitude();
};

#endif