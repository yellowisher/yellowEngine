#include "Component.hpp"


Component::Component()
{

}


Component::Component(GameObject* gameObject)
{
	this->gameObject = gameObject;
}


Component::~Component()
{

}
