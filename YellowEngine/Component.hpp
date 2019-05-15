#ifndef __H_COMPONENT__
#define __H_COMPONENT__

class GameObject;

class Component
{
	friend GameObject;

public:
	void setActive(bool active);
	bool getActive();

protected:
	Component();
	Component(GameObject* gameObject);
	virtual ~Component();

private:
	GameObject* _gameObject;
	bool _active;
};

#endif