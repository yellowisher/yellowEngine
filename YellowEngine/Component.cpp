#include "Component.hpp"
#include "GameObject.hpp"


Component::Component(GameObject* gameObject) :gameObject(gameObject), transform(gameObject->transform)
{
	_active = true;
}


Component::~Component()
{
}


void Component::setActive(bool active)
{
	_active = active;
}


bool Component::getActive()
{
	return _active;
}
