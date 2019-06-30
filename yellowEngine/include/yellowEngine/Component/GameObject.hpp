#ifndef __H_GAMEOBJECT__
#define __H_GAMEOBJECT__

#include <list>
#include <string>

#include "yellowEngine/Component/Transform.hpp"

namespace yellowEngine
{
	class Component;

	class GameObject
	{
	public:
		static GameObject* find(const std::string& name);

		Transform* const transform;

		GameObject();
		GameObject(const char* name);
		GameObject(const GameObject& copy);
		~GameObject();
		GameObject& operator=(const GameObject& copy) = delete;

		void setActive(bool active);
		bool getActive();
		std::string getName();
		void setName(const char* name);

		template <typename T> T* getComponent();
		template <typename T> T* addComponent();
		Component* addComponent(const std::string& type);
		void removeComponent(Component* target);
		const std::list<Component*>& getComponents() { return _components; }

	private:
		std::list<Component*> _components;
		std::string _name;
		bool _active;
	};


	template <typename T> T* GameObject::getComponent()
	{
		if (!std::is_base_of<Component, T>::value)return nullptr;

		for (auto component : _components)
		{
			if (Component::getTypeNameOf<T>() == component->getTypeName())
			{
				return dynamic_cast<T*>(component);
			}
			// dynamic_cast version
			//T* target = dynamic_cast<T*>(component);
			//if (target)return target;
		}

		return nullptr;
	}


	template <typename T> T* GameObject::addComponent()
	{
		if (!std::is_base_of<Component, T>::value)return nullptr;

		T* component = new T(this);
		component->onCreate();
		_components.push_back(component);
		return component;
	}
}

#endif