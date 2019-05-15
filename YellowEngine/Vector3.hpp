#ifndef __H_VECTOR3__
#define __H_VECTOR3__

class Vector3
{
public:
	union
	{
		float v[3];
		struct
		{
			float x;
			float y;
			float z;
		};
	};

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3& vector);
	~Vector3();

	Vector3 operator+(const Vector3& vector) const;
	Vector3 operator-(const Vector3& vector) const;
	Vector3 operator*(float value) const;
	Vector3 operator/(float value) const;
	bool operator<(const Vector3& vector) const;
	bool operator==(const Vector3& vector) const;
	bool operator!=(const Vector3& vector) const;

	void normalize();
	float magnitude();
};

#endif