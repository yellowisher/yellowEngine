#include <vector>

#include "Unit.hpp"

using namespace yellowEngine;

COMPONENT_IMPL(Unit)

static const int DYING_FRAME = 30;
static const Vector3 INITIAL_SCALE = Vector3(0.01f, 0.01f, 0.01f);

Unit::BaseUnitType Unit::getBaseType(UnitType type)
{
	if (type < Unit_Cavalry_Start) return BaseUnit_Infantry;
	else return BaseUnit_Cavalry;
}


AnimationClip* Unit::getClip(UnitType unit, ClipType clip)
{
	static Model* infantryModel = Model::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Model\\units\\infantry.fbx");
	static Model* cavalryModel = Model::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Model\\units\\cavalry.fbx");

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


void Unit::enterAttackRange(Collider* other)
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


void Unit::exitAttackRange(Collider* other)
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
			assert(_state == State_Dying || _state == State_NotInitialized);
		}
	}
}


void Unit::update()
{
	static const int attackFrame = 14;

	switch (_state)
	{
		case State_Moving:
			transform->translate(transform->getForward() * -moveSpeed);
			break;
		case State_Attacking:
			// (_frame == 0) means didn't attack in this attack animation
			if (_frame == 0)
			{
				if (_animator->getFrame() >= attackFrame)
				{
					_attackingTarget->takeDamage(damage[_attackingTarget->getBaseType()]);
					_frame++;
				}
			}
			else
			{
				if (++_frame == attackDelay)
				{
					_frame = 0;
					_animator->play(getClip(type, Clip_Attack));
				}
			}
			break;
		case State_Dying:
			if (!_animator->isPlaying())
			{
				if (--_frame == 0)
				{
					delete(gameObject);
					return;
				}
				else
				{
					float factor = (float)_frame / DYING_FRAME;
					transform->setScale(INITIAL_SCALE * factor);
				}
			}
			break;
	}

	// to ensure same units die same time
	// units die after last update
	if (_dead)
	{
		dieSelf();
	}
}


int Unit::modifyDamage(int damage)
{
	return Utils::max(damage - defense, 0);
}


void Unit::initialize(int team)
{
	this->team = team;
	_animator = gameObject->addComponent<Animator>();
	_animator->setSpeed(0.5f);
	_dead = false;
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
	_dead = true;
}


void Unit::dieSelf()
{
	_dead = false;
	_state = State_Dying;
	_frame = DYING_FRAME;
	_animator->play(getClip(type, deathClip));
	Collider* collider = gameObject->getComponent<Collider>();
	gameObject->removeComponent(collider);
}