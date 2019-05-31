#include <type_traits>

#include "Component.hpp"
#include "GameObject.hpp"


GameObject::GameObject() :_name("GameObject"), transform(new Transform(this))
{
}


GameObject::GameObject(const char* name) : _name(name), transform(new Transform(this))
{
}


GameObject::GameObject(const GameObject& copy) : _name(copy._name + "(clone)"), transform(new Transform(this))
{
	// TODO: copy all members
}


GameObject::~GameObject()
{
	while (!_components.empty())
	{
		Component* last = _components.back();
		last->onDestroy();
		_components.pop_back();
		delete(last);
	}
	transform->onDestroy();
	delete(transform);
}


void GameObject::setActive(bool active)
{
	_active = active;
}


bool GameObject::getActive()
{
	return _active;
}


std::string GameObject::getName()
{
	return _name;
}