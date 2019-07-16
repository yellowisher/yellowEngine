#include <vector>

#include "Unit.hpp"

using namespace yellowEngine;

COMPONENT_IMPL(Unit)

Unit::BaseUnitType Unit::getBaseType(UnitType type)
{
	if (type < Unit_Cavalry_Start) return BaseUnit_Infantry;
	else return BaseUnit_Cavalry;
}


AnimationClip* Unit::getClip(UnitType unit, ClipType clip)
{
	static Model* infantryModel = Model::create("./res/Mesh/Units/infantry.fbx");
	static Model* cavalryModel = Model::create("./res/Mesh/Units/cavalry.fbx");

	static std::string unitNames[Num_Units] = {
		"infantry","spear","archer", "crossbow", "infantry","cavalry", "cavalry", "cavalry_lance" };
	static std::string clipNames[Num_Clips] = {
		"_idle", "_walk", "_run", "_attack", "_death_a", "_death_b"
	};
	static AnimationClip* clips[Num_Units][Num_Clips] = { nullptr, };

	if (clips[0][0] == nullptr)
	{
		for (int u = 0; u < Num_Units; u++)
		{
			Model* model = isInfantry(u) ? infantryModel : cavalryModel;
			for (int c = 0; c < Num_Clips; c++)
			{
				std::string name = unitNames[u] + clipNames[c];
				clips[u][c] = model->getClip(name.c_str());
				if (c == Clip_Run || c == Clip_Walk)
				{
					clips[u][c]->_isLooping = true;
				}
			}
		}
	}
	
	return clips[unit][clip];
}


void Unit::onCreate()
{
	_state = State_NotInitialized;
}


void Unit::onCollisionEnter(Collider* other)
{
	IDamageable* damageable = other->gameObject->getComponent<IDamageable>();
	if (damageable != nullptr)
	{
		if (damageable->team == team) return;

		if (_state == State_Attacking)
		{
			_targets.push_back(damageable);
		}
		else if (_state == State_Moving)
		{
			attack(damageable);
		}
	}
}


void Unit::onCollisionExit(Collider* other)
{
	IDamageable* damageable = other->gameObject->getComponent<IDamageable>();
	if (damageable != nullptr)
	{
		if (damageable->team == team) return;

		if (_state == State_Attacking)
		{
			if (_attackingTarget == damageable)
			{
				// if attacking target is gone -> find new target
				if (_targets.empty())
				{
					move();
				}
				else
				{
					auto targetIt = _targets.begin();
					float minMagnitude = Utils::inf;

					for (auto it = _targets.begin(); it != _targets.end(); ++it)
					{
						float magnitude =
							(transform->getWorldPosition() - ((Transform*)((*it)->getTransform()))->getWorldPosition()).magnitude();

						if (magnitude < minMagnitude)
						{
							minMagnitude = magnitude;
							targetIt = it;
						}
					}

					attack((*targetIt));
					_targets.erase(targetIt);
				}
			}
			else
			{
				// if attacking other -> erase from target list
				for (auto it = _targets.begin(); it != _targets.end(); ++it)
				{
					if (*it == damageable)
					{
						_targets.erase(it);
						return;
					}
				}
			}
		}
		else
		{
			// there was possible attack target but was not attacking;
			// should not reach here
			assert(false);
		}
	}
}


void Unit::update()
{
	static const int attackFrame = 16;

	switch (_state)
	{
		case State_Moving:
			transform->translate(transform->getForward() * -moveSpeed);
			break;
		case State_Attacking:
			if (++_frame == attackFrame)
			{
				_attackingTarget->takeDamage(damage[_attackingTarget->baseType]);
			}
			else if (_frame == (getClip(type, Clip_Attack)->_frameCount + attackDelay))
			{
				_frame = 0;
				_animator->play(getClip(type, Clip_Attack));
			}
			break;
		case State_Dying:
			if (!_animator->isPlaying())
			{
				delete(gameObject);
			}
			break;
	}
}


int Unit::modifyDamage(int damage)
{
	return Utils::max(damage - defense, 0);
}


void Unit::initialize(int team)
{
	this->team = team;
	_animator = gameObject->getComponent<Animator>();
	_attackingTarget = nullptr;
	move();
}


void Unit::move()
{
	_state = State_Moving;
	_animator->play(getClip(type, moveClip));
}


void Unit::attack(IDamageable* target)
{
	_state = State_Attacking;
	_animator->play(getClip(type, Clip_Attack));
	_attackingTarget = target;
	_frame = 0;
}


void Unit::die()
{
	_state = State_Dying;
	_animator->play(getClip(type, deathClip));
	Collider* collider = gameObject->getComponent<Collider>();
	gameObject->removeComponent(collider);
}