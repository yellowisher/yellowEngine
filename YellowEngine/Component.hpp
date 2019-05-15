#ifndef __H_COMPONENT__
#define __H_COMPONENT__

#include "GameObject.hpp"

class Component
{
public:
	Component(GameObject* gameObject);
	virtual ~Component();

	void setActive();

private:
	Component();

	GameObject* _gameObject;
	bool _isActive;
};

#endif