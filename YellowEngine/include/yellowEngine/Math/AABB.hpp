#ifndef __H_AABB__
#define __H_AABB__

#include "yellowEngine/Math/Vector3.hpp"

class AABB
{
	friend class Collider;
public:
	static AABB combine(const AABB& a, const AABB& b);

	AABB();
	AABB(Vector3 min, Vector3 max);
	AABB(const AABB& copy);
	~AABB();

	AABB& operator=(const AABB& other);

	bool isCollideWith(const AABB& other);
	bool contains(const AABB& other);

	float perimeter();
	float volume();
	AABB& expand(float factor);

	inline const Vector3& min() { return _min; }
	inline const Vector3& max() { return _max; }
	const Vector3& operator[](int i) const
	{
		if (i == 0)return _min;
		return _max;
	}

private:
	Vector3 _min;
	Vector3 _max;
};

#endif