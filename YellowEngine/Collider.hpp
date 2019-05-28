#ifndef __H_COLLIDER__
#define __H_COLLIDER__

#include "Component.hpp"
#include "Event.hpp"

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

	virtual bool isCollide(Collider* other) = 0;
	virtual Type getType();
	void notify(Event event, void* sender) override;

protected:
	EventListener transformChangeListener;

	virtual void onTransformChange() = 0;
};

#endif