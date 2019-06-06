#include <string>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Math/AABB.hpp"


namespace yellowEngine
{
	AABB::AABB()
	{
	}


	AABB::AABB(Vector3 min, Vector3 max) : _min(min), _max(max)
	{
	}


	AABB::AABB(const AABB& copy) : _min(copy._min), _max(copy._max)
	{
	}


	AABB::~AABB()
	{
	}


	AABB AABB::combine(const AABB& a, const AABB& b)
	{
		Vector3 cmin = Vector3(Utils::min(a._min.x, b._min.x), Utils::min(a._min.y, b._min.y), Utils::min(a._min.z, b._min.z));
		Vector3 cmax = Vector3(Utils::max(a._max.x, b._max.x), Utils::max(a._max.y, b._max.y), Utils::max(a._max.z, b._max.z));
		return AABB(cmin, cmax);
	}


	AABB& AABB::operator=(const AABB& other)
	{
		_min = other._min;
		_max = other._max;

		return *this;
	}


	bool AABB::isCollideWith(const AABB& other)
	{
		if (_max.x < other._min.x || _min.x > other._max.x) return false;
		if (_max.y < other._min.y || _min.y > other._max.y) return false;
		if (_max.z < other._min.z || _min.z > other._max.z) return false;
		return true;
	}


	bool AABB::contains(const AABB& other)
	{
		return
			_min.x <= other._min.x && _min.y <= other._min.y && _min.z <= other._min.z &&
			_max.x >= other._max.x && _max.y >= other._max.y && _max.z >= other._max.z;
	}


	float AABB::perimeter()
	{
		return (_max.x - _min.x) + (_max.y - _min.y) + (_max.z - _min.z) * 4.0f;
	}


	float AABB::volume()
	{
		return (_max.x - _min.x) * (_max.y - _min.y) * (_max.z - _min.z);
	}


	AABB& AABB::expand(float factor)
	{
		Vector3 amount = (_max - _min) * factor * 0.5f;
		_max += amount;
		_min -= amount;

		return *this;
	}
}