#ifndef __H_BUILDING__
#define __H_BUILDING__

#include <yellowEngine/yellowEngine.hpp>

#include "IDamageable.hpp"

using namespace yellowEngine;

class Building : public Component, public IDamageable
{
	BEGIN_COMPONENT(Building)
		PROPERTY(Building, int, hp, "HP")
		PROPERTY(Building, int, team, "Team")
	END_COMPONENT

public:
	Building(GameObject* gameObject) : Component(gameObject) {}
	virtual ~Building() {}

	virtual void die();
	virtual void* getTransform() { return transform; }

private:

};

#endif