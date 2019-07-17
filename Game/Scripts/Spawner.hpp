#ifndef __H_SPAWNER__
#define __H_SPAWNER__

#include "yellowEngine/yellowEngine.hpp"

#include "Unit.hpp"

using namespace yellowEngine;

class Spawner : public Component, public IUpdatable
{
	BEGIN_COMPONENT(Spawner)
		PROPERTY(Spawner, int, team, "Team")
	END_COMPONENT

public:
	Spawner(GameObject* gameObject) :Component(gameObject) {};
	~Spawner() {};

	void start() override;
	void spawn(Unit::UnitType type);

	int team;

private:
	static Material* getMaterial(int team);
	static GameObject* units[Unit::Num_Units];

	void setMaterial(Transform* target);
};

#endif