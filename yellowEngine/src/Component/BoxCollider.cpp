#include <glad/glad.h>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Collision/ColliderManager.hpp"
#include "yellowEngine/Math/Matrix.hpp"
#include "yellowEngine/Component/Transform.hpp"
#include "yellowEngine/Component/SphereCollider.hpp"
#include "yellowEngine/Component/BoxCollider.hpp"


namespace yellowEngine
{
	COMPONENT_IMPL(BoxCollider)


	BoxCollider::BoxCollider(GameObject* gameObject) :
		Collider(gameObject),
		_pointsChanged(true)
	{
	}


	BoxCollider::~BoxCollider()
	{
	}


	void BoxCollider::onCreate()
	{
		Collider::onCreate();
	}


	void BoxCollider::onDestroy()
	{
		Collider::onDestroy();
	}


	void BoxCollider::onValueChanged()
	{
		set(_min.x, _min.y, _min.z, _max.x, _max.y, _max.z);
	}


	void BoxCollider::set(float minx, float miny, float minz, float maxx, float maxy, float maxz)
	{
		_localPoints[Right_Top_Back] = Vector3(maxx, maxy, maxz);
		_localPoints[Right_Top_Front] = Vector3(maxx, maxy, minz);
		_localPoints[Right_Bottom_Back] = Vector3(maxx, miny, maxz);
		_localPoints[Right_Bottom_Front] = Vector3(maxx, miny, minz);
		_localPoints[Left_Top_Back] = Vector3(minx, maxy, maxz);
		_localPoints[Left_Top_Front] = Vector3(minx, maxy, minz);
		_localPoints[Left_Bottom_Back] = Vector3(minx, miny, maxz);
		_localPoints[Left_Bottom_Front] = Vector3(minx, miny, minz);

		_pointsChanged = true;
		ColliderManager::getInstance()->colliderUpdated(this);
	}


	void BoxCollider::setSize(float x, float y, float z)
	{
		float hx = x / 2.0f;
		float hy = y / 2.0f;
		float hz = z / 2.0f;

		set(-hx, -hy, -hz, hx, hy, hz);
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


	void BoxCollider::initSize(const AABB& bounds)
	{
		_min = bounds.min;
		_max = bounds.max;
		set(bounds.min.x, bounds.min.y, bounds.min.z, bounds.max.x, bounds.max.y, bounds.max.z);
	}


	void BoxCollider::fillRenderingPoints(std::vector<Vector3>& lines)
	{
		updatePoints();

		// if any bit is 0, pair original point with original point | bit
		lines.clear();
		for (int i = 0; i < Num_Points; i++)
		{
			for (int b = 0; b < 3; b++)
			{
				int bit = 1 << b;
				if (!(i & bit))
				{
					lines.push_back(_worldPoints[i]);
					lines.push_back(_worldPoints[i | bit]);
				}
			}
		}
	}


	void BoxCollider::updatePoints()
	{
		// if points changed, re-calculate bounding box first
		if (_pointsChanged)
		{
			_pointsChanged = false;
			Matrix matrix = transform->getMatrix();

			for (int i = 0; i < Num_Points; i++)
			{
				_worldPoints[i] = matrix * _localPoints[i];
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

			// 6 face normal axis
			if (!projectionOverlap(otherBox))return false;
			if (!otherBox->projectionOverlap(this))return false;

			// and 9 cross edge axis
			for (int a = 0; a < 3; a++)
			{
				Vector3 va = _worldPoints[0] - _worldPoints[1LL << a];
				for (int b = 0; b < 3; b++)
				{
					Vector3 vb = otherBox->_worldPoints[0] - otherBox->_worldPoints[1LL << b];

					Vector3 axis = Vector3::cross(va, vb);
					if (axis == Vector3::zero) continue;

					Vector2 boundA = getProjectedBounds(Vector3::zero, axis);
					Vector2 boundB = otherBox->getProjectedBounds(Vector3::zero, axis);

					if (boundA.min > boundB.max || boundA.max < boundB.min) return false;
				}
			}

			return true;
		}
		else if (other->getType() == Type_Sphere)
		{
			SphereCollider* otherSphere = (SphereCollider*)other;
			updatePoints();

			Vector3 worldCenter = otherSphere->getWorldCenter();
			Vector3 center = transform->getInverseMatrix() * worldCenter;

			Vector3& max = _localPoints[Right_Top_Back];
			Vector3& min = _localPoints[Left_Bottom_Front];
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

			Vector3 worldClosestPoint = transform->getMatrix() * closestPoint;

			Vector3 scale = other->transform->getMatrix().extractScale();
			float radius = Utils::max(scale.x, scale.y, scale.z) * otherSphere->radius;

			return (worldCenter - worldClosestPoint).magnitude() < radius * radius;
		}
		return false;
	}


	bool BoxCollider::projectionOverlap(BoxCollider* other)
	{
		// check for 3 axis (x, y, z)
		for (int i = 0; i < 3; i++)
		{
			Vector3 a = _worldPoints[0];
			Vector3 b = _worldPoints[1LL << i];

			Vector3 ab = b - a;
			float min = 0;
			float max = ab.magnitude();

			Vector2 bounds = other->getProjectedBounds(a, ab);
			if (bounds.min > max || bounds.max < min) return false;
		}
		return true;
	}


	Vector2 BoxCollider::getProjectedBounds(const Vector3& a, const Vector3& ab)
	{
		Vector2 bounds = Vector2(Utils::inf, -Utils::inf);

		for (int i = 0; i < Num_Points; i++)
		{
			Vector3 ap = _worldPoints[i] - a;
			float value = ab * ap;
			if (value > bounds.max) bounds.max = value;
			if (value < bounds.min) bounds.min = value;
		}

		return bounds;
	}


	void BoxCollider::onTransformChange()
	{
		_pointsChanged = true;
	}
}