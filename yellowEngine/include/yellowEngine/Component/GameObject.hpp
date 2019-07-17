#ifndef __H_GAMEOBJECT__
#define __H_GAMEOBJECT__

#include <list>
#include <string>

#include "yellowEngine/Component/Transform.hpp"

namespace yellowEngine
{
	class Component;
	class Collider;

	class GameObject
	{
	public:
		static GameObject* find(const std::string& name);

		Transform* const transform;

		GameObject();
		GameObject(const char* name);
		~GameObject();
		GameObject(const GameObject& copy) = delete;
		GameObject& operator=(const GameObject& copy) = delete;

		void onCollisionEnter(Collider* other);
		void onCollisionExit(Collider* other);
		GameObject* clone();

		void setActive(bool active);
		bool getActive();
		std::string getName();
		void setName(const char* name);

		template <typename T> T* getComponent();
		template <typename T> T* addComponent();
		Component* addComponent(const std::string& type, bool noStart = true);
		void removeComponent(Component* target);
		const std::list<Component*>& getComponents() { return _components; }

	private:
		GameObject* cloneObjects();
		void cloneComponents(GameObject* original);

		// delete without remove request to parent
		void deleteSelf(bool silent);

		std::list<Component*> _components;
		std::string _name;
		bool _active;
	};


	template <typename T> T* GameObject::getComponent()
	{
		// c++ rtti version
		for (auto component : _components)
		{
			T* target = dynamic_cast<T*>(component);
			if (target)return target;
		}

		// custom rtti version
		//const auto& childToParent = Component::getParents();
		//auto typeName = Component::getTypeNameOf<T>();
		//if (childToParent.find(typeName) == childToParent.end()) return nullptr;

		//for (auto component : _components)
		//{
		//	if (Component::isInstanceOf(component->getTypeName(), typeName))
		//	{
		//		return component;
		//	}
		//}

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