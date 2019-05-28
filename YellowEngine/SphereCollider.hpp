#ifndef __SPHERECOLLIDER__
#define __SPHERECOLLIDER__

#include "Collider.hpp"

class SphereCollider : public Collider
{
public:
	SphereCollider(GameObject* gameObject);
	~SphereCollider();

	Type getType() override;
	bool isCollide(Collider* other) override;
};

#endif