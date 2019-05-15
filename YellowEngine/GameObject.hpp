#ifndef __H_GAMEOBJECT__
#define __H_GAMEOBJECT__

#include <list>
#include <string>

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const char* name);
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

#endif