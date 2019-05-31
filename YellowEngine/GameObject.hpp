#ifndef __H_GAMEOBJECT__
#define __H_GAMEOBJECT__

#include <list>
#include <string>

#include "Transform.hpp"

class Component;

class GameObject
{
public:
	Transform* const transform;

	GameObject();
	GameObject(const char* name);
	GameObject(const GameObject& copy);
	~GameObject();

	void setActive(bool active);
	bool getActive();
	std::string getName();

	template <typename T> T* getComponent();
	template <typename T> T* addComponent();

private:
	std::list<Component*> _components;
	std::string _name;
	bool _active;
};


template <typename T> T* GameObject::getComponent()
{
	if (!std::is_base_of<Component, T>::value)return nullptr;

	// TODO: implement RTTI rather than dynamic_cast
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

	T* component = new T(this);
	Component* comp = static_cast<Component*>(component);
	comp->onCreate();
	_components.push_back(comp);
	// add to update list?
	return component;
}
#endif