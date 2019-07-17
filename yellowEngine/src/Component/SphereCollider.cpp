#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Component/Camera.hpp"
#include "yellowEngine/Component/Transform.hpp"
#include "yellowEngine/Component/Collider.hpp"
#include "yellowEngine/Component/SphereCollider.hpp"


namespace yellowEngine
{
	COMPONENT_IMPL(SphereCollider)


	SphereCollider::SphereCollider(GameObject* gameObject) :
		Collider(gameObject),
		radius(0.5f)
	{
	}


	SphereCollider::~SphereCollider()
	{
	}


	void SphereCollider::onCreate()
	{
		Collider::onCreate();
	}


	void SphereCollider::onDestroy()
	{
		Collider::onDestroy();
	}


	Collider::Type SphereCollider::getType()
	{
		static Type type = Type_Sphere;
		return type;
	}


	void SphereCollider::fillRenderingPoints(std::vector<Vector3>& lines)
	{
		static const int pointsCount = 72;
		static std::vector<float> sins;
		static std::vector<float> coss;

		if (sins.empty())
		{
			sins.resize(pointsCount);
			coss.resize(pointsCount);
			for (int i = 0; i < pointsCount; i++)
			{
				float rad = Utils::pi * 2.0f * ((float)i / pointsCount);
				sins[i] = sinf(rad);
				coss[i] = cosf(rad);
			}
		}

		lines.clear();
		Matrix matrix = transform->getMatrix();
		for (int v = 0; v < 3; v++)
		{
			int x = (v + 1) % 3;
			int y = (v + 2) % 3;
			Vector3 prevPoint;
			prevPoint.v[x] = coss[pointsCount - 1] * radius;
			prevPoint.v[y] = sins[pointsCount - 1] * radius;
			prevPoint = matrix * prevPoint;

			for (int i = 0; i < pointsCount; i++)
			{
				Vector3 point;
				point.v[x] = coss[i] * radius;
				point.v[y] = sins[i] * radius;
				point = matrix * point;

				lines.push_back(prevPoint);
				lines.push_back(point);

				prevPoint = point;
			}
		}

		// background circle
		Matrix ref = Camera::currentCamera->getVMatrix() * matrix;
		Vector3 x = Vector3(ref[0], ref[4], ref[8]);
		Vector3 y = Vector3(ref[1], ref[5], ref[9]);
		x.normalize();
		y.normalize();

		Vector3 prevPoint;
		prevPoint += x * coss[pointsCount - 1] * radius;
		prevPoint += y * sins[pointsCount - 1] * radius;
		prevPoint = matrix * prevPoint;

		for (int i = 0; i < pointsCount; i++)
		{
			Vector3 point;
			point += x * coss[i] * radius;
			point += y * sins[i] * radius;
			point = matrix * point;

			lines.push_back(prevPoint);
			lines.push_back(point);

			prevPoint = point;
		}
	}


	AABB SphereCollider::getBoundingBox()
	{
		Vector3 center = transform->getWorldPosition();
		Vector3 scale = transform->getWorldScale();
		Vector3 min = center - scale * radius;
		Vector3 max = center + scale * radius;

		return AABB(min, max);
	}


	// TODO: add center field and adjust size
	void SphereCollider::initSize(const AABB& bounds)
	{
	}


	bool SphereCollider::isCollideWith(Collider* other)
	{
		if (other->getType() == Type_Box)
		{
			return other->isCollideWith(this);
		}
		else if (other->getType() == Type_Sphere)
		{
			SphereCollider* otherSphere = (SphereCollider*)other;
			float dist = (transform->getWorldPosition() - other->transform->getWorldPosition()).magnitude();
			float r = otherSphere->radius * otherSphere->transform->getWorldScale().x + radius * transform->getWorldScale().x;
			return  dist < r * r;
		}
		return false;
	}


	void SphereCollider::onTransformChange()
	{
	}
}