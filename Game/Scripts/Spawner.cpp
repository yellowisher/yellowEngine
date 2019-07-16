#include "Spawner.hpp"


COMPONENT_IMPL(Spawner)

GameObject* Spawner::units[Unit::Num_Units] = { nullptr, };

Material* Spawner::getMaterial(int team)
{
	static Material* materials[2] = { nullptr };
	if (materials[0] == nullptr)
	{
		materials[0] = new Material("blue team");
		materials[0]->setProperty("u_Material.diffuse", Texture::create("./res/Mesh/Units/Materials/blue.tga"));
		
		materials[0] = new Material("red team");
		materials[0]->setProperty("u_Material.diffuse", Texture::create("./res/Mesh/Units/Materials/red.tga"));
	}

	return materials[team];
}


void Spawner::onCreate()
{
	if (units[0] == nullptr)
	{
		for (int i = 0; i < Unit::Num_Units; i++)
		{
			units[i] = GameObject::find("Unit" + std::to_string(i));
			if (units[i]) units[i]->setActive(false);
		}
	}
}


void Spawner::spawn(Unit::UnitType type)
{
	GameObject* newObject = units[type]->clone();
	Unit* unit = newObject->getComponent<Unit>();
	unit->initialize(team);

	newObject->transform->setPosition(transform->position);
	newObject->transform->setRotation(transform->rotation);
}