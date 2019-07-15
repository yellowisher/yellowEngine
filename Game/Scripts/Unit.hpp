#ifndef __H_UNIT__
#define __H_UNIT__

#include <string>
#include <yellowEngine/yellowEngine.hpp>

#include "IDamageable.hpp"

using namespace yellowEngine;

class Unit : public Component, public IDamageable
{
public:
	enum UnitType
	{
		Unit_Sword,
		Unit_Spear,
		Unit_Bow,
		Unit_Crossbow,
		Unit_Armor,
		Unit_Cavalry_Start, // infantry / cavalry separator
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

	enum MeshType
	{
		Mesh_Body,
		Mesh_RightHand,
		Mesh_LeftHand,
		Mesh_Head,
		Mesh_Back,
		Mesh_Horse,
		Num_Meshes
	};

	struct UnitInfo
	{
		UnitType type;
		std::string meshes[Num_Meshes];

		int hp;
		int defense;
		ClipType moveClip;
		ClipType deathClip;
		float moveSpeed;
		int damage[Num_BaseUnits];
	};

	static BaseUnitType getBaseType(UnitType type);
	static UnitInfo getUnitInfo(UnitType type);

	Unit(GameObject* gameObject) :Component(gameObject) {};
	~Unit() {};

	void set(UnitType type);

	UnitType type;
};

#endif