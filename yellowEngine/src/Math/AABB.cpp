#include <string>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Math/AABB.hpp"


namespace yellowEngine
{
	AABB::AABB()
	{
	}


	AABB::AABB(Vector3 min, Vector3 max) : min(min), max(max)
	{
	}


	AABB::AABB(const AABB& copy) : min(copy.min), max(copy.max)
	{
	}


	AABB::~AABB()
	{
	}


	AABB AABB::combine(const AABB& a, const AABB& b)
	{
		Vector3 cmin = Vector3(Utils::min(a.min.x, b.min.x), Utils::min(a.min.y, b.min.y), Utils::min(a.min.z, b.min.z));
		Vector3 cmax = Vector3(Utils::max(a.max.x, b.max.x), Utils::max(a.max.y, b.max.y), Utils::max(a.max.z, b.max.z));
		return AABB(cmin, cmax);
	}


	AABB& AABB::operator=(const AABB& other)
	{
		min = other.min;
		max = other.max;

		return *this;
	}


	bool AABB::isCollideWith(const AABB& other)
	{
		if (max.x < other.min.x || min.x > other.max.x) return false;
		if (max.y < other.min.y || min.y > other.max.y) return false;
		if (max.z < other.min.z || min.z > other.max.z) return false;
		return true;
	}


	bool AABB::contains(const AABB& other)
	{
		return
			min.x <= other.min.x && min.y <= other.min.y && min.z <= other.min.z &&
			max.x >= other.max.x && max.y >= other.max.y && max.z >= other.max.z;
	}


	float AABB::perimeter()
	{
		return (max.x - min.x) + (max.y - min.y) + (max.z - min.z) * 4.0f;
	}


	float AABB::volume()
	{
		return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
	}


	AABB& AABB::expand(float factor)
	{
		Vector3 amount = (max - min) * factor * 0.5f;
		max += amount;
		min -= amount;

		return *this;
	}
}