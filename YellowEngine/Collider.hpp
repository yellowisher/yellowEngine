#ifndef __H_COLLIDER__
#define __H_COLLIDER__

#include <vector>

#include "Component.hpp"
#include "Vector3.hpp"
#include "Event.hpp"
#include "EventListener.hpp"
#include "Mesh.hpp"
#include "AABB.hpp"

class ColliderManager;

class Collider : public Component, public INotifiable
{
public:
	enum Type
	{
		Type_None = -1,
		Type_Box,
		Type_Sphere
	};

	Collider(GameObject* gameObject);
	virtual ~Collider();

	virtual bool isCollideWith(Collider* other) = 0;
	virtual Type getType() = 0;
	virtual AABB getBoundingBox() = 0;
	virtual void fillRenderingPoints(std::vector<Vector3>& lines) = 0;

	virtual void onCreate() override;
	virtual void onDestroy() override;

protected:
	EventListener _transformChangeListener;

	virtual void onTransformChange() = 0;

private:
	void notify(Event event, void* sender) override;
};

#endif