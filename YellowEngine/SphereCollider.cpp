#include "Transform.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"


SphereCollider::SphereCollider(GameObject* gameObject) :
	Collider(gameObject),
	radius(0.5f)
{
}


SphereCollider::~SphereCollider()
{
}


Collider::Type SphereCollider::getType()
{
	static Type type = Type_Sphere;
	return type;
}


void SphereCollider::fillRenderingPoints(std::vector<Vector3>& lines)
{

}



AABB SphereCollider::getBoundingBox()
{
	Vector3 center = transform->getWorldPosition();
	Vector3 min = center - Vector3(radius, radius, radius);
	Vector3 max = center + Vector3(radius, radius, radius);

	return AABB(min, max);
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
		return  dist < (otherSphere->radius + radius)*(otherSphere->radius + radius);
	}
	return false;
}


void SphereCollider::onTransformChange()
{
}