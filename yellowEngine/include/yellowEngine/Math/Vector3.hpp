#ifndef __H_VECTOR3__
#define __H_VECTOR3__

namespace yellowEngine
{
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

		static const Vector3& zero;
		static const Vector3& up;
		static const Vector3& right;
		static const Vector3& forward;

		static Vector3 lerp(const Vector3& v0, const Vector3& v1, float delta);
		static Vector3 cross(const Vector3& v0, const Vector3 v1);

		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const Vector3& vector);
		~Vector3();
		void set(float x, float y, float z);

		Vector3 operator+(const Vector3& vector) const;
		Vector3 operator-(const Vector3& vector) const;
		Vector3 operator*(float value) const;
		Vector3 operator/(float value) const;

		Vector3& operator= (const Vector3& vector);
		Vector3& operator+= (const Vector3& vector);
		Vector3& operator-= (const Vector3& vector);

		float operator*(const Vector3& vector) const;

		bool operator<(const Vector3& vector) const;
		bool operator==(const Vector3& vector) const;
		bool operator!=(const Vector3& vector) const;

		void normalize();
		float magnitude();
	};
}

#endif