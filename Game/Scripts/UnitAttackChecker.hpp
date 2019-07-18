#ifndef __H_UNITATTACKCHECKER__
#define __H_UNITATTACKCHECKER__

#include <yellowEngine/yellowEngine.hpp>

#include "Unit.hpp"

using namespace yellowEngine;

class UnitAttackChecker : public Component
{
	BEGIN_COMPONENT(UnitAttackChecker)
	END_COMPONENT

public:
	void onCreate() override;
	void onCollisionEnter(Collider* other) override;
	void onCollisionExit(Collider* other) override;

private:
	Unit* _unit;
};

#endif