#include "UnitAttackChecker.hpp"


COMPONENT_IMPL(UnitAttackChecker)


void UnitAttackChecker::onCreate()
{
	_unit = transform->getParent()->gameObject->getComponent<Unit>();
	if (_unit == nullptr) delete(this);
}


void UnitAttackChecker::onCollisionEnter(Collider* other)
{
	if (_unit)_unit->enterAttackRange(other);
}


void UnitAttackChecker::onCollisionExit(Collider* other)
{
	if (_unit)_unit->exitAttackRange(other);
}