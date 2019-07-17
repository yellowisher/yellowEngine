#include "Spawner.hpp"


COMPONENT_IMPL(Spawner)

GameObject* Spawner::units[Unit::Num_Units] = { nullptr, };

Material* Spawner::getMaterial(int team)
{
	static Material* materials[2] = { nullptr };
	if (materials[0] == nullptr)
	{
		materials[0] = new Material("blue team");
		materials[0]->setProperty("u_Material.diffuse", Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Model\\units\\materials\\TT_RTS_Units_blue.tga"));
		
		materials[1] = new Material("red team");
		materials[1]->setProperty("u_Material.diffuse", Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Model\\units\\materials\\TT_RTS_Units_red.tga"));
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

	Unit* unit = newObject->getComponent<Unit>();
	unit->initialize(team);

	newObject->transform->setPosition(transform->getWorldPosition());
	newObject->transform->setRotation(transform->getWorldRotation());
	setMaterial(unit->transform);
}


void Spawner::setMaterial(Transform* target)
{
	auto meshRenderer = target->gameObject->getComponent<MeshRenderer>();
	if (meshRenderer != nullptr)
	{
		meshRenderer->set(nullptr, getMaterial(team));
	}

	for (auto child : target->getChildren())
	{
		setMaterial(child);
	}
}