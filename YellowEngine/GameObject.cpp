#include <type_traits>

#include "Component.hpp"
#include "GameObject.hpp"


GameObject::GameObject() :_name("GameObject")
{
	_parent = nullptr;
}


GameObject::GameObject(const char* name) : _name(name)
{
	_parent = nullptr;
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


template <typename T> T* GameObject::getComponent()
{
	if (!std::is_base_of<Component, T>::value)return nullptr;

	// implementing kind of RTTI would be better
	for (auto component : _components)
	{
		T* target = dynamic_cast<T*>(component);
		if (target)return target;
	}

	return nullptr;
}


template <typename T> T* GameObject::addComponent()
{
	if (!std::is_base_of<Component, T>::value)return nullptr;

	Component component = new T(this);
	_components.push_back(component);
	// add to update list?
	return component;
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