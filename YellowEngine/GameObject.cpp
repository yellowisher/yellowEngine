#include <type_traits>

#include "Component.hpp"
#include "GameObject.hpp"


GameObject::GameObject() :_name("GameObject"), transform(new Transform(this))
{
	_parent = nullptr;
}


GameObject::GameObject(const char* name) : _name(name), transform(new Transform(this))
{
	_parent = nullptr;
}


GameObject::GameObject(const GameObject& copy) :_name(copy._name + "(clone)"), transform(new Transform(this))
{
	// TODO: copy all members
}


GameObject::~GameObject()
{
	while (!_children.empty())
	{
		GameObject* last = _children.back();
		_children.pop_back();
		delete(last);
	}

	while (!_components.empty())
	{
		Component* last = _components.back();
		_components.pop_back();
		delete(last);
	}
	if (_parent)_parent->removeChild(this);
}


void GameObject::addChild(GameObject* child)
{
	if (child->_parent)child->_parent->removeChild(child);
	child->_parent = this;
	_children.push_back(child);
}


void GameObject::removeChild(GameObject* child)
{
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		if (*it == child)
		{
			_children.erase(it);
			child->_parent = nullptr;
			return;
		}
	}
}