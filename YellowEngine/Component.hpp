#ifndef __H_COMPONENT__
#define __H_COMPONENT__

#include "GameObject.hpp"

class Component
{
public:
	Component();
	Component(GameObject* gameObject);
	virtual ~Component();

	void setActive();

private:
	GameObject* gameObject;
	bool isActive;
};

#endif