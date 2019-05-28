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


bool SphereCollider::isCollide(Collider* other)
{
	if (other->getType() == Type_Box)
	{
		return other->isCollide(this);
	}
	else if (other->getType() == Type_Sphere)
	{
		SphereCollider* otherSphere = (SphereCollider*)other;
		float dist = (transform->getWorldPosition() - other->transform->getWorldPosition()).magnitude();
		return  dist < (otherSphere->radius + radius)*(otherSphere->radius + radius);
	}
	return false;
}