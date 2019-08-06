#ifndef __H_ARROW__
#define __H_ARROW__

#include <yellowEngine/yellowEngine.hpp>

#include "Unit.hpp"

using namespace yellowEngine;

class Arrow : public Component, public IUpdatable
{
public:
	Arrow(GameObject* gameObject) :Component(gameObject) {}
	~Arrow() {};

	void update() override;

	void initialize(Transform* target, int* damages, Unit* owner);

private:
	void updateTarget();
	Vector3 getArrowPosition();

	int _damages[Unit::Num_Units];

	Unit* _owner;
	Transform* _target;
	Unit* _targetUnit;
	Vector3 _lastTarget;
};

#endif