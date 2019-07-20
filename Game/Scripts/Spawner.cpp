#include "Spawner.hpp"


COMPONENT_IMPL(Spawner)

GameObject* Spawner::units[Unit::Num_Units] = { nullptr, };


GameObject* Spawner::spawn(Unit::UnitType type, Vector3 position, Quaternion rotation, int team)
{
	GameObject* newObject = units[type]->clone();
	newObject->transform->setRotation(rotation);

	if (team < 2)
	{
		Unit* unit = newObject->getComponent<Unit>();
		unit->initialize(team);
	}
	else
	{
		newObject->removeComponent(newObject->getComponent<Unit>());
		delete(newObject->transform->findChild("Attack Range")->gameObject);
	}
	setMaterial(newObject->transform, team);

	newObject->transform->setPosition(position);
	return newObject;
}


Material* Spawner::getMaterial(int team)
{
	static Material* materials[4] = { nullptr };
	if (materials[0] == nullptr)
	{
		materials[0] = new Material("blue team");
		materials[0]->setProperty("u_Material.diffuse", Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Model\\units\\materials\\TT_RTS_Units_blue.tga"));
		
		materials[1] = new Material("red team");
		materials[1]->setProperty("u_Material.diffuse", Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Model\\units\\materials\\TT_RTS_Units_red.tga"));

		materials[2] = new Material("placing material");
		materials[2]->setProperty("u_Material.diffuseColor", Vector3(0.2f, 1.0f, 1.0f));

		materials[3] = new Material("not placing material");
		materials[3]->setProperty("u_Material.diffuseColor", Vector3(1.0f, 0.2f, 0.2f));
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
	spawn(type, transform->getWorldPosition(), transform->getWorldRotation(), team);
}


void Spawner::setMaterial(Transform* target, int team)
{
	auto meshRenderer = target->gameObject->getComponent<MeshRenderer>();
	if (meshRenderer != nullptr)
	{
		meshRenderer->set(nullptr, getMaterial(team));
	}

	for (auto child : target->getChildren())
	{
		setMaterial(child, team);
	}
}