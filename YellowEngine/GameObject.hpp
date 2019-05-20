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

	template <typename T> T* getComponent();
	template <typename T> T* addComponent();

	void addChild(GameObject* child);
	void removeChild(GameObject* child);

private:
	std::list<Component*> _components;
	std::list<GameObject*> _children;
	std::string _name;
	GameObject* _parent;
};


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

	T* component = new T(this);
	_components.push_back(static_cast<Component*>(component));
	// add to update list?
	return component;
}
#endif