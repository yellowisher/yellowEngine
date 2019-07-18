#ifndef __H_TRACERANGECHECKER__
#define __H_TRACERANGECHECKER__

#include <yellowEngine/yellowEngine.hpp>

#include "Unit.hpp"

using namespace yellowEngine;

class TraceRangeChecker : public Component
{
	BEGIN_COMPONENT(TraceRangeChecker)
	END_COMPONENT

public:
	void onCreate() override;
	void onCollisionEnter(Collider* other) override;
	void onCollisionExit(Collider* other) override;

private:
	Unit* _unit;
};

#endif