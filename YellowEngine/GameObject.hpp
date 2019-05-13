#ifndef __H_GAMEOBJECT__
#define __H_GAMEOBJECT__

using namespace std;

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
	void addChild(GameObject* child);
	void removeChild(GameObject* child);
	void removeAllChildren();

private:
	list<Component*> components;
	string name;
};

#endif