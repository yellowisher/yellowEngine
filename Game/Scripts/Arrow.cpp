#include "Arrow.hpp"


void Arrow::initialize(Transform* target, int* damages, Unit* owner)
{
	_target = target;
	_owner = owner;
	_targetUnit = target->gameObject->getComponent<Unit>();
	for (int i = 0; i < Unit::Num_BaseUnits; i++)
	{
		_damages[i] = damages[i];
	}
	updateTarget();

	Vector3 v0 = transform->getForward();
	Vector3 v1 = transform->getWorldPosition() - _lastTarget;
	Vector3 a = Vector3::cross(v0, v1);
	float w = sqrtf(v0.magnitude() * v1.magnitude()) + v0 * v1;
	Quaternion rotation = Quaternion(a.x, a.y, a.z, w);
	transform->rotate(rotation);
}


void Arrow::updateTarget()
{
	static Vector3 offset = Vector3(0, 0.5, 0);

	if (_target == nullptr) return;

	if (_targetUnit != nullptr)
	{
		if (_targetUnit->isDying())
		{
			_lastTarget = _target->getWorldPosition() + offset;
			_target = nullptr;
			return;
		}
	}

	_lastTarget = _target->getWorldPosition() + offset;
}


Vector3 Arrow::getArrowPosition()
{
	static Vector3 arrowOffset = transform->getForward() * 0.01f;
	return transform->getWorldPosition() + arrowOffset;
}


void Arrow::update()
{
	static float moveSpeed = 0.24f;

	if (_owner != nullptr && _owner->isDying())
	{
		_owner = nullptr;
	}
	Vector3 worldPosition = getArrowPosition();

	Vector3 direction = _lastTarget - worldPosition;
	float magnitude = direction.magnitude();
	direction.normalize();

	transform->translate(direction * moveSpeed);
	Vector3 newWorldPosition = getArrowPosition();
	float passed = (_lastTarget - worldPosition).x * (_lastTarget - newWorldPosition).x;
	if (magnitude < 0.01f || passed < 0)
	{
		if (_target != nullptr)
		{
			auto dmgbl = _target->gameObject->getComponent<IDamageable>();
			dmgbl->takeDamage(_damages[dmgbl->getBaseType()], _owner);
		}
		delete(gameObject);
		return;
	}

	updateTarget();
}
