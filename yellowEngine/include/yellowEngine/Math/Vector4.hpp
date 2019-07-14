#ifndef __H_VECTOR4__
#define __H_VECTOR4__

namespace yellowEngine
{
	class Vector3;
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
		Vector4(const Vector3& vector, float w);
		Vector4(const Vector4& vector);
		~Vector4();
		void set(float x, float y, float z, float w);

		Vector4 operator+(const Vector4& vector) const;
		Vector4 operator-(const Vector4& vector) const;
		Vector4 operator*(float value) const;
		Vector4 operator/(float value) const;

		Vector4& operator= (const Vector3& vector);
		Vector4& operator= (const Vector4& vector);
		Vector4& operator+= (const Vector4& vector);
		Vector4& operator-= (const Vector4& vector);

		bool operator<(const Vector4& vector) const;
		bool operator==(const Vector4& vector) const;
		bool operator!=(const Vector4& vector) const;

		void normalize();
		float magnitude();
	};
}

#endif