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
			PROPERTY(SphereCollider, Vector3, _center, "Center")
		END_COMPONENT

	public:
		SphereCollider(GameObject* gameObject);
		~SphereCollider();

		void onCreate() override;
		void onDestroy() override;
		void onActive(bool active) override { Collider::onActive(active); }
		void onValueChanged() override;

		Type getType() override;
		bool isCollideWith(Collider* other) override;
		void fillRenderingPoints(std::vector<Vector3>& lines) override;
		AABB getBoundingBox() override;

		Vector3 getWorldCenter();

		float radius;

	private:
		void initSize(const AABB& bounds) override;
		void onTransformChange();

		Vector3 _center;
		Vector3 _worldCenter;
		bool _centerDirty;
	};
}

#endif