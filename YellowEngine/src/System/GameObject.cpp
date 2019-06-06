#include <type_traits>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/System/GameObject.hpp"


namespace yellowEngine
{
	GameObject::GameObject() :
		_name("GameObject"),
		transform(new Transform(this)),
		_active(true)
	{
	}


	GameObject::GameObject(const char* name) :
		_name(name),
		transform(new Transform(this)),
		_active(true)
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
}