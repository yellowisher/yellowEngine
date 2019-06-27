#ifndef __SPHERECOLLIDER__
#define __SPHERECOLLIDER__

#include "yellowEngine/Component/Collider.hpp"
#include <iostream>

namespace yellowEngine
{
	class SphereCollider : public Collider
	{
		BEGIN_COMPONENT(SphereCollider)
			PROPERTY(SphereCollider, float, radius, "Radius")
		END_COMPONENT

	public:
		SphereCollider(GameObject* gameObject);
		~SphereCollider();

		Type getType() override;
		bool isCollideWith(Collider* other) override;
		void fillRenderingPoints(std::vector<Vector3>& lines) override;
		AABB getBoundingBox() override;

		float radius;

	private:
		void initSize(const AABB& bounds) override;
		void onTransformChange();
	};
}

#endif