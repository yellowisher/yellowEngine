#include "Component.hpp"


Component::Component(GameObject* gameObject)
{
	_gameObject = gameObject;
	_active = true;
}


Component::~Component()
{
}


GameObject* Component::getGameObject()
{
	return _gameObject;
}


void Component::setActive(bool active)
{
	_active = active;
}


bool Component::getActive()
{
	return _active;
}
