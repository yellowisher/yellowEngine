#include "SphereCollider.hpp"


SphereCollider::SphereCollider(GameObject* gameObject) :Collider(gameObject)
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


bool SphereCollider::isCollide(Collider* other)
{
	return false;
}