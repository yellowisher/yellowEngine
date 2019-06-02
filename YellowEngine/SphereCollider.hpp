#ifndef __SPHERECOLLIDER__
#define __SPHERECOLLIDER__

#include "Collider.hpp"

class SphereCollider : public Collider
{
public:
	SphereCollider(GameObject* gameObject);
	~SphereCollider();

	Type getType() override;
	bool isCollideWith(Collider* other) override;
	void setColliderRenderingData() override;
	AABB getBoundingBox() override;

	float radius;

private:
	void onTransformChange();
};

#endif