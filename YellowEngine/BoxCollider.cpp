#include <gl/glew.h>

#include "Utils.hpp"
#include "Transform.hpp"
#include "Matrix.hpp"
#include "SphereCollider.hpp"
#include "BoxCollider.hpp"


BoxCollider::BoxCollider(GameObject* gameObject) :
	Collider(gameObject),
	_pointsChanged(true)
{
	setSize(1.0f, 1.0f, 1.0f);
}


BoxCollider::~BoxCollider()
{
}


void BoxCollider::set(float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
	_points[Right_Top_Back]     = Vector3(maxx, maxy, maxz);
	_points[Right_Top_Front]    = Vector3(maxx, maxy, minz);
	_points[Right_Bottom_Back]  = Vector3(maxx, miny, maxz);
	_points[Right_Bottom_Front] = Vector3(maxx, miny, minz);
	_points[Left_Top_Back]      = Vector3(minx, maxy, maxz);
	_points[Left_Top_Front]     = Vector3(minx, maxy, minz);
	_points[Left_Bottom_Back]   = Vector3(minx, miny, maxz);
	_points[Left_Bottom_Front]  = Vector3(minx, miny, minz);

	_pointsChanged = true;
}


void BoxCollider::setSize(float x, float y, float z)
{
	float hx = x / 2.0f;
	float hy = y / 2.0f;
	float hz = z / 2.0f;

	set(-hx, -hy, -hz, hx, hy, hz);
}


void BoxCollider::setSize(Mesh* mesh)
{
	Mesh::Bounds bounds = mesh->getBounds();
	Vector3& min = bounds.min;
	Vector3& max = bounds.max;

	set(min.x, min.y, min.z, max.x, max.y, max.z);
}


Collider::Type BoxCollider::getType()
{
	static Type type = Type_Box;
	return type;
}


AABB BoxCollider::getBoundingBox()
{
	updatePoints();

	Vector3 min = Vector3(Utils::inf, Utils::inf, Utils::inf);
	Vector3 max = Vector3(-Utils::inf, -Utils::inf, -Utils::inf);

	for (int i = 0; i < Num_Points; i++)
	{
		min.x = Utils::min(min.x, _worldPoints[i].x);
		min.y = Utils::min(min.y, _worldPoints[i].y);
		min.z = Utils::min(min.z, _worldPoints[i].z);

		max.x = Utils::max(max.x, _worldPoints[i].x);
		max.y = Utils::max(max.y, _worldPoints[i].y);
		max.z = Utils::max(max.z, _worldPoints[i].z);
	}

	return AABB(min, max);
}


// if any bit is 0, pair original point with original point | bit
void BoxCollider::setColliderRenderingData()
{
	_colRenderer.setData(Renderer::Box(_points[Left_Bottom_Front], _points[Right_Top_Back]));
}


void BoxCollider::updatePoints()
{
	// if points changed, re-calculate bounding box first
	if (_pointsChanged)
	{
		_pointsChanged = false;
		Matrix matrix = transform->getMatrix();
		Matrix sMatrix = transform->getSMatrix();

		for (int i = 0; i < Num_Points; i++)
		{
			_worldPoints[i] = matrix * _points[i];
			_scalePoints[i] = sMatrix * _points[i];
		}
	}
}


bool BoxCollider::isCollideWith(Collider* other)
{
	if (other->getType() == Type_Box)
	{
		BoxCollider* otherBox = (BoxCollider*)other;
		updatePoints();
		otherBox->updatePoints();

		if (!projectionOverlap(otherBox))return false;
		if (!otherBox->projectionOverlap(this))return false;
		return true;
	}
	else if (other->getType() == Type_Sphere)
	{
		SphereCollider* otherSphere = (SphereCollider*)other;
		updatePoints();

		Vector3 center = other->transform->getWorldPosition();
		center = transform->getInverseTRMatrix() * center;

		Vector3& max = _scalePoints[Right_Top_Back];
		Vector3& min = _scalePoints[Left_Bottom_Front];
		Vector3 closestPoint;

		if (center.x < min.x)closestPoint.x = min.x;
		else if (center.x > max.x)closestPoint.x = max.x;
		else closestPoint.x = center.x;

		if (center.y < min.y)closestPoint.y = min.y;
		else if (center.y > max.y)closestPoint.y = max.y;
		else closestPoint.y = center.y;

		if (center.z < min.z)closestPoint.z = min.z;
		else if (center.z > max.z)closestPoint.z = max.z;
		else closestPoint.z = center.z;

		Vector3 v = (center - closestPoint);
		return (center - closestPoint).magnitude() < otherSphere->radius * otherSphere->radius;
	}
	return false;
}


bool BoxCollider::projectionOverlap(BoxCollider* other)
{
	// check for 3 axis (x, y, z)
	for (int i = 0; i < 3; i++)
	{
		Vector3 a = _worldPoints[0];
		Vector3 b = _worldPoints[1 << i];

		Vector3 ab = b - a;
		float min = 0;
		float max = ab.magnitude();

		Vector2 bounds = other->getProjectedBounds(a, ab);
		if (bounds.min > max || bounds.max < min)return false;
	}
	return true;
}


Vector2 BoxCollider::getProjectedBounds(Vector3& a, Vector3& ab)
{
	Vector2 bounds = Vector2(Utils::inf, -Utils::inf);

	for (int i = 0; i < Num_Points; i++)
	{
		Vector3 ap = _worldPoints[i] - a;
		float value = ab * ap;
		if (value > bounds.max)bounds.max = value;
		if (value < bounds.min)bounds.min = value;
	}

	return bounds;
}


void BoxCollider::onTransformChange()
{
	_pointsChanged = true;
}