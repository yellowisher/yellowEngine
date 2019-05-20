#ifndef __H_COMPONENT__
#define __H_COMPONENT__

class GameObject;

class Component
{
	friend class GameObject;

public:
	Component(GameObject* gameObject);
	virtual ~Component();

	GameObject* getGameObject();
	void setActive(bool active);
	bool getActive();

private:
	GameObject* _gameObject;
	bool _active;
};

#endif