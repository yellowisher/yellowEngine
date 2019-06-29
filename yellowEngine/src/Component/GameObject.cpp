#include <type_traits>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Component/GameObject.hpp"


namespace yellowEngine
{
	GameObject::GameObject() :
		_name("GameObject"),
		transform(new Transform(this)),
		_active(true)
	{
		_components.push_back(transform);
	}


	GameObject::GameObject(const char* name) :
		_name(name),
		transform(new Transform(this)),
		_active(true)
	{
		_components.push_back(transform);
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


	void GameObject::setName(const char* name)
	{
		_name = name;
	}


	Component* GameObject::addComponent(const std::string& type)
	{
		Component* component = Component::createComponent(type, this);
		component->onCreate();
		_components.push_back(component);
		return component;
	}


	void GameObject::removeComponent(Component* target)
	{
		for (auto it = _components.begin(); it != _components.end(); ++it)
		{
			if (*it == target)
			{
				_components.erase(it);
				delete(target);
				return;
			}
		}
	}
}