#ifndef __BOX_COLLIDER__
#define __BOX_COLLIDER__

#include "Mesh.hpp"
#include "Collider.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

class BoxCollider : public Collider
{
public:
	BoxCollider(GameObject* gameObject);
	~BoxCollider();

	Type getType() override;
	bool isCollideWith(Collider* other) override;
	void calcRenderingData() override;
	const AABB getAABB() override;

	void setSize(float x, float y, float z);
	void setSize(Mesh* mesh);

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

	bool _pointsChanged;
	Vector3 _points[Num_Points];
	Vector3 _scalePoints[Num_Points];
	Vector3 _worldPoints[Num_Points];

	void set(float minx, float miny, float minz, float maxx, float maxy, float maxz);
	bool projectionOverlap(BoxCollider* other);
	void updatePoints();
	Vector2 getProjectedBounds(Vector3& a, Vector3& ab);
	void onTransformChange() override;
};

#endif