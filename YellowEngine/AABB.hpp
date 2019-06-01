#ifndef __H_AABB__
#define __H_AABB__

#include "Vector3.hpp"

class AABB
{
	friend class Collider;
public:
	AABB(Vector3 min, Vector3 max);
	AABB(const AABB& copy);
	~AABB();

	bool isCollideWith(const AABB& other);
	const Vector3& min;
	const Vector3& max;
	const Vector3& m(int i) const
	{
		if (i == 0)return min;
		else return max;
	}

private:
	enum Point
	{
		Right_Top_Back,
		Right_Top_Front,
		Right_Bottom_Back,
		Right_Bottom_Front,
		Left_Top_Back,
		Left_Top_Front,
		Left_Bottom_Back,
		Left_Bottom_Front,
		Num_Points
	};

	Vector3 _points[Num_Points];
};

#endif