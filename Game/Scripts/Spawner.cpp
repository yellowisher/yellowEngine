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


void Spawner::start()
{
	if (units[0] == nullptr)
	{
		for (int i = 0; i < Unit::Num_Units; i++)
		{
			units[i] = GameObject::find("Unit" + std::to_string(i));
			units[i]->setActive(false);
		}
	}
}


void Spawner::spawn(Unit::UnitType type)
{
	GameObject* newObject = units[type]->clone();

	newObject->transform->setPosition(transform->getWorldPosition());
	newObject->transform->setRotation(transform->getWorldRotation());

	Unit* unit = newObject->getComponent<Unit>();
	unit->initialize(team);
}