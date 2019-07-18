#ifndef __H_UNIT__
#define __H_UNIT__

#include <string>
#include <vector>
#include <yellowEngine/yellowEngine.hpp>

#include "IDamageable.hpp"

using namespace yellowEngine;

class ArrowShooter;

class Unit : public Component, public IDamageable, public IUpdatable
{
	BEGIN_COMPONENT(Unit)
		PROPERTY(Unit, UnitType, type,						"Type")
		PROPERTY(Unit, int,		 hp,						"HP")
		PROPERTY(Unit, int,		 defense,					"Defense")
		PROPERTY(Unit, float,	 moveSpeed,					"Move Speed")
		PROPERTY(Unit, int,		 damage[BaseUnit_Infantry],	"Infantry Damage")
		PROPERTY(Unit, int,		 damage[BaseUnit_Cavalry],	"Cavalry Damage")
		PROPERTY(Unit, int,		 damage[BaseUnit_Building],	"Building Damage")
		PROPERTY(Unit, int,		 attackDelay,				"Attack Delay")
		PROPERTY(Unit, ClipType, moveClip,					"Move Clip")
		PROPERTY(Unit, ClipType, deathClip,					"Death Clip")

		BEGIN_ENUM(Unit, UnitType)
			ENUM("Sword")
			ENUM("Separ")
			ENUM("Bow")
			ENUM("Crossbow")
			ENUM("Armor")
			ENUM("Cavalry_Hammer")
			ENUM("Cavalry_Sword")
			ENUM("Cavalry_Lance")
		END_ENUM

		BEGIN_ENUM(Unit, ClipType)
			ENUM("Idle")
			ENUM("Walk")
			ENUM("Run")
			ENUM("Attack")
			ENUM("Death_A")
			ENUM("Death_B")
		END_ENUM
	END_COMPONENT

public:
	enum UnitType
	{
		Unit_Sword,
		Unit_Spear,
		Unit_Bow,
		Unit_Crossbow,
		Unit_Armor,
		Unit_Cavalry_Start,
		Unit_Cavalry_Hammer = Unit_Cavalry_Start,
		Unit_Cavalry_Sword,
		Unit_Cavalry_Spear,
		Num_Units
	};

	enum BaseUnitType
	{
		BaseUnit_Infantry,
		BaseUnit_Cavalry,
		BaseUnit_Building,
		Num_BaseUnits
	};

	enum ClipType
	{
		Clip_Idle,
		Clip_Walk,
		Clip_Run,
		Clip_Attack,
		Clip_Death_A,
		Clip_Death_B,
		Num_Clips
	};

	static bool isInfantry(int type) { return type < Unit_Cavalry_Start; }
	static BaseUnitType getBaseType(UnitType type);
	static AnimationClip* getClip(UnitType unit, ClipType clip);
	
	Unit(GameObject* gameObject) :Component(gameObject) {};
	~Unit() {};

	void onCreate() override;
	void update() override;
	void die() override;
	int modifyDamage(int damage) override;

	void enterTraceRange(Collider* other);
	void exitTraceRange(Collider* other);

	void enterAttackRange(Collider* other);
	void exitAttackRange(Collider* other);
	
	void initialize(int team);
	void dieSelf();
	void* getTransform() { return transform; }
	int getBaseType() override { return getBaseType(type); }
	bool isDying() { return _state == State_Dying; }

	UnitType type;
	int defense;
	float moveSpeed;
	int damage[Num_BaseUnits];
	int attackDelay;
	ClipType moveClip;
	ClipType deathClip;

private:
	enum State
	{
		State_NotInitialized,
		State_Moving,
		State_Attacking,
		State_Dying
	};

	void move();
	void attack(IDamageable* target);

	bool _dead;
	State _state;
	Animator* _animator;
	IDamageable* _attackingTarget;
	int _frame;
	std::vector<IDamageable*> _targets;
	ArrowShooter* _arrowShooter;
};

#endif