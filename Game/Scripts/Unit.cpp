#include <vector>

#include "Unit.hpp"


using namespace yellowEngine;


Unit::BaseUnitType Unit::getBaseType(Unit::UnitType type)
{
	if (type < Unit_Cavalry_Start) return BaseUnit_Infantry;
	else return BaseUnit_Cavalry;
}


Unit::UnitInfo Unit::getUnitInfo(Unit::UnitType type)
{
	static UnitInfo infos[Num_Units];
	static bool initialized = false;

	if (!initialized)
	{
		initialized = true;

		// TODO:: store unit information in .json or implement prefab system
		infos[Unit_Sword].type = Unit_Sword;
		infos[Unit_Sword].meshes[Mesh_Body] = "Body_02c_$AssimpFbx$_Translation";
		infos[Unit_Sword].meshes[Mesh_RightHand] = "w_short_sword";
		infos[Unit_Sword].meshes[Mesh_LeftHand] = "shield_03";
		infos[Unit_Sword].meshes[Mesh_Head] = "Head_06a";
		infos[Unit_Sword].meshes[Mesh_Back] = "";
		infos[Unit_Sword].meshes[Mesh_Horse] = "";
		infos[Unit_Sword].hp = 100;
		infos[Unit_Sword].defense = 5;
		infos[Unit_Sword].moveClip = Clip_Run;
		infos[Unit_Sword].deathClip = Clip_Death_A;
		infos[Unit_Sword].moveSpeed = 0.05;
		infos[Unit_Sword].damage[BaseUnit_Infantry] = 20;
		infos[Unit_Sword].damage[BaseUnit_Cavalry] = 20;
		infos[Unit_Sword].damage[BaseUnit_Building] = 20;
	}
}


void Unit::set(Unit::UnitType type)
{
	static std::vector<std::string> bodyPath[2] = { {},{} };

	this->type = type;
	this->baseType = getBaseType(type);
}
