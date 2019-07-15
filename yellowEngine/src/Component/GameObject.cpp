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


	GameObject::~GameObject()
	{
		for (auto it = _components.rbegin(); it != _components.rend(); ++it)
		{
			(*it)->onDestroy();
			delete(*it);
		}
	}


	GameObject* GameObject::clone(bool first)
	{
		GameObject* newObject = new GameObject(_name.c_str());

		for (auto child : transform->getChildren())
		{
			GameObject* newChild = child->gameObject->clone(false);
			newObject->transform->addChild(newChild->transform);
		}

		for (auto component : _components)
		{
			if (strcmp(component->getTypeName(), "Transform") == 0)
			{
				transform->clone(newObject->transform);
				newObject->transform->onValueChanged();
			}
			else
			{
				Component* newComponent = newObject->addComponent(component->getTypeName());
				component->clone(newComponent);
				newComponent->onValueChanged();
			}
		}

		if (first)
		{
			transform->getParent()->addChild(newObject->transform);
		}
		return newObject;
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


	GameObject* GameObject::find(const std::string& name)
	{
		for (auto child : Transform::Root->getChildren())
		{
			if (child->gameObject->_name == name)
			{
				return child->gameObject;
			}
		}
		return nullptr;
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
				(*it)->onDestroy();
				_components.erase(it);
				delete(target);
				return;
			}
		}
	}
}