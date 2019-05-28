#include "Transform.hpp"
#include "Collider.hpp"


Collider::Collider(GameObject* gameObject) :
	Component(gameObject),
	transformChangeListener(this)
{
	transform->transformChangeNotifier.addListener(&transformChangeListener);
}


Collider::~Collider()
{
}


Collider::Type Collider::getType()
{
	static Type type = Type_None;
	return type;
}


void Collider::notify(Event event, void* sender)
{
	switch (event)
	{
		case Event_TransformChanged:
			onTransformChange();
			break;
	}
}
