#include "Utils.hpp"
#include "Transform.hpp"
#include "Matrix.hpp"
#include "BoxCollider.hpp"


BoxCollider::BoxCollider(GameObject* gameObject) :Collider(gameObject), _pointsChanged(true)
{
	setSize(1.0f, 1.0f, 1.0f);
}


BoxCollider::~BoxCollider()
{
}


void BoxCollider::setSize(float x, float y, float z)
{
	float hx = x / 2.0f;
	float hy = y / 2.0f;
	float hz = z / 2.0f;

	_points[Left][Top][Front]     = Vector3(-hx, hy, hz);
	_points[Left][Top][Back]      = Vector3(-hx, hy, -hz);
	_points[Left][Bottom][Front]  = Vector3(-hx, -hy, hz);
	_points[Left][Bottom][Back]   = Vector3(-hx, -hy, -hz);
	_points[Right][Top][Front]    = Vector3(hx, hy, hz);
	_points[Right][Top][Back]     = Vector3(hx, hy, -hz);
	_points[Right][Bottom][Front] = Vector3(hx, -hy, hz);
	_points[Right][Bottom][Back]  = Vector3(hx, -hy, -hz);

	_pointsChanged = true;
}


Collider::Type BoxCollider::getType()
{
	static Type type = Type_Box;
	return type;
}


bool BoxCollider::isCollide(Collider* other)
{
	if (other->getType() == Type_Box)
	{
		BoxCollider* bOther = (BoxCollider*)other;
		if (!isCollideWith(bOther))return false;
		if (!bOther->isCollideWith(this))return false;
		return true;
	}
	else if (other->getType() == Type_Sphere)
	{

	}
	return false;
}


bool BoxCollider::isCollideWith(BoxCollider* other)
{
	// if points changed, re-calculate bounding box first
	if (_pointsChanged)
	{
		_pointsChanged = false;
		Matrix matrix = transform->getMatrix();

		for (int i = 0; i < Num_TotalPoint; i++)
		{
			_worldPoints[i] = matrix * _p[i];
		}
	}


	// check for 3 axis
	for (int i = 0; i < 3; i++)
	{
		Vector3 a = _worldPoints[2 * i];
		Vector3 b = _worldPoints[2 * i + 1];

		Vector3 ab = b - a;
		float min = 0;
		float max = b.magnitude();
		if (max < 0)
		{
			min = max;
			max = 0;
		}

		Vector2 bounds = other->getProjectedBounds(a, ab);
		if (bounds.min < max || bounds.max > min)return false;
	}
	return true;
}


Vector2 BoxCollider::getProjectedBounds(Vector3& a, Vector3& ab)
{
	Vector2 bounds = Vector2(Utils::inf, -Utils::inf);

	for (int i = 0; i < Num_TotalPoint; i++)
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