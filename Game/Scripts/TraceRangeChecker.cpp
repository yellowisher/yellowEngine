#include "TraceRangeChecker.hpp"


COMPONENT_IMPL(TraceRangeChecker)


void TraceRangeChecker::onCreate()
{
	_unit = transform->getParent()->gameObject->getComponent<Unit>();
}


void TraceRangeChecker::onCollisionEnter(Collider* other)
{
	_unit->enterTraceRange(other);
}


void TraceRangeChecker::onCollisionExit(Collider* other)
{
	_unit->exitTraceRange(other);
}