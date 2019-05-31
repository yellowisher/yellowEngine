#include <string>

#include "AABB.hpp"


AABB::AABB(Vector3 min, Vector3 max): min(_points[Left_Bottom_Front]), max(_points[Right_Top_Back])
{
	_points[Right_Top_Back]     = Vector3(max.x, max.y, max.z);
	_points[Right_Top_Front]    = Vector3(max.x, max.y, min.z);
	_points[Right_Bottom_Back]  = Vector3(max.x, min.y, max.z);
	_points[Right_Bottom_Front] = Vector3(max.x, min.y, min.z);
	_points[Left_Top_Back]      = Vector3(min.x, max.y, max.z);
	_points[Left_Top_Front]     = Vector3(min.x, max.y, min.z);
	_points[Left_Bottom_Back]   = Vector3(min.x, min.y, max.z);
	_points[Left_Bottom_Front]  = Vector3(min.x, min.y, min.z);
}


AABB::AABB(const AABB& copy) : min(_points[Left_Bottom_Front]), max(_points[Right_Top_Back])
{
	memcpy(_points, copy._points, sizeof(_points));
}


AABB::~AABB()
{
}


bool AABB::isCollideWith(const AABB& other)
{
	if (max.x < other.min.x || min.x > other.max.x) return false;
	if (max.y < other.min.y || min.y > other.max.y) return false;
	if (max.z < other.min.z || min.z > other.max.z) return false;
	return true;
}