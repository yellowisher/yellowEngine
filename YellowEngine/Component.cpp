#include "Component.hpp"

Component::Component()
{

}


Component::Component(GameObject* gameObject)
{
	_gameObject = gameObject;
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
