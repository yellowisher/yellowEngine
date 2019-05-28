#ifndef __BOX_COLLIDER__
#define __BOX_COLLIDER__

#include "Collider.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

class BoxCollider : public Collider
{
public:
	BoxCollider(GameObject* gameObject);
	~BoxCollider();

	Type getType() override;
	bool isCollide(Collider* other) override;

	void setSize(float x, float y, float z);

private:
	enum Point
	{
		Left = 0,
		Top = 0,
		Front = 0,
		Right = 1,
		Bottom = 1,
		Back = 1,
		Num_Point = 2,
		Num_TotalPoint = Num_Point * Num_Point * Num_Point
	};

	union
	{
		Vector3 _points[Num_Point][Num_Point][Num_Point];
		Vector3 _p[Num_TotalPoint];
	};
	Vector3 _worldPoints[Num_TotalPoint];
	bool _pointsChanged;

	bool isCollideWith(BoxCollider* other);
	Vector2 getProjectedBounds(Vector3& a, Vector3& ab);
	void onTransformChange() override;
};

#endif