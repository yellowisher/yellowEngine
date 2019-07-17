#ifndef __BOX_COLLIDER__
#define __BOX_COLLIDER__

#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Component/Collider.hpp"
#include "yellowEngine/Math/Vector2.hpp"
#include "yellowEngine/Math/Vector3.hpp"

namespace yellowEngine
{
	class BoxCollider : public Collider
	{
		BEGIN_COMPONENT(BoxCollider)
			PROPERTY(BoxCollider, Vector3, _min, "Min")
			PROPERTY(BoxCollider, Vector3, _max, "Max")
		END_COMPONENT

	public:
		BoxCollider(GameObject* gameObject);
		~BoxCollider();
		virtual void onCreate() override;
		virtual void onDestroy() override;
		virtual void onValueChanged() override;
		virtual void onActive(bool active) override { Collider::onActive(active); }

		Type getType() override;
		bool isCollideWith(Collider* other) override;
		void fillRenderingPoints(std::vector<Vector3>& lines) override;
		AABB getBoundingBox() override;
		void setSize(float x, float y, float z);

	private:
		enum Point
		{
			Right_Top_Back,
			Right_Top_Front,
			Right_Bottom_Back,
			Right_Bottom_Front,
			Left_Top_Back,
			Left_Top_Front,
			Left_Bottom_Back,
			Left_Bottom_Front,
			Num_Points
		};

		void set(float minx, float miny, float minz, float maxx, float maxy, float maxz);
		void initSize(const AABB& bounds) override;
		bool projectionOverlap(BoxCollider* other);
		void updatePoints();
		Vector2 getProjectedBounds(const Vector3& a, const Vector3& ab);
		void onTransformChange() override;

		bool _pointsChanged;
		Vector3 _localPoints[Num_Points];
		Vector3 _worldPoints[Num_Points];

		Vector3 _min;
		Vector3 _max;
	};
}

#endif