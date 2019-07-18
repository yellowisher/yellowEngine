#ifndef __H_ARROWSHOOTER__
#define __H_ARROWSHOOTER__

#include <yellowEngine/yellowEngine.hpp>

using namespace yellowEngine;

class Unit;

class ArrowShooter : public Component
{
	BEGIN_COMPONENT(ArrowShooter)
	END_COMPONENT

public:
	void initialize(Unit* unit);
	void fireArrow(Transform* target);

private:
	Unit* _unit;
};

#endif