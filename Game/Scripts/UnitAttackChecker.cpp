#include "UnitAttackChecker.hpp"


COMPONENT_IMPL(UnitAttackChecker)


void UnitAttackChecker::onCreate()
{
	_unit = transform->getParent()->gameObject->getComponent<Unit>();
}


void UnitAttackChecker::onCollisionEnter(Collider* other)
{
	_unit->enterAttackRange(other);
}


void UnitAttackChecker::onCollisionExit(Collider* other)
{
	_unit->exitAttackRange(other);
}