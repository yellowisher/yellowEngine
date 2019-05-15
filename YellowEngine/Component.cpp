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
