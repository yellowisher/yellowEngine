#include <type_traits>
#include <cassert>

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

	// because of SkinnedMeshRenderer, two pass cloning
	// first, clone all child objects
	// second, fill all components
	GameObject* GameObject::clone()
	{
		GameObject* newObject = cloneObjects();
		transform->getParent()->addChild(newObject->transform);

		newObject->cloneComponents(this);

		return newObject;
	}


	void GameObject::onCollisionEnter(Collider* other)
	{
		for (auto component : _components)
		{
			component->onCollisionEnter(other);
		}
	}


	void GameObject::onCollisionExit(Collider* other)
	{
		for (auto component : _components)
		{
			component->onCollisionExit(other);
		}
	}


	GameObject* GameObject::cloneObjects()
	{
		GameObject* newObject = new GameObject(_name.c_str());

		for (auto child : transform->getChildren())
		{
			GameObject* newChild = child->gameObject->cloneObjects();
			newObject->transform->addChild(newChild->transform);
		}

		return newObject;
	}


	void GameObject::cloneComponents(GameObject* original)
	{
		for (auto component : original->_components)
		{
			Component* newComponent = nullptr;
			if (strcmp(component->getTypeName(), "Transform") == 0)
			{
				newComponent = transform;
			}
			else
			{
				newComponent = addComponent(component->getTypeName());
			}

			component->clone(newComponent);
			newComponent->onValueChanged();
		}

		assert(transform->getChildCount() == original->transform->getChildCount());
		for (int i = 0; i < transform->getChildCount(); i++)
		{
			transform->getChild(i)->gameObject->cloneComponents(original->transform->getChild(i)->gameObject);
		}
	}


	void GameObject::setActive(bool active)
	{
		_active = active;
		for (auto component : _components)
		{
			component->onActive(active);
		}
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