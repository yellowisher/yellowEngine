#ifndef __H_GAMEMANAGER__
#define __H_GAMEMANAGER__

#include <vector>

#include <yellowEngine/yellowEngine.hpp>

#include "Spawner.hpp"
#include "Unit.hpp"

using namespace yellowEngine;

class GameManager : public IUpdatable
{
public:
	GameManager() {}
	~GameManager() {}

	void start() override;
	void update() override;

	int gold;
	
private:
	int selected;
	Transform* placing;
	Image* _images[Unit::Num_Units];
	std::vector<Spawner*> _spawners[2];
};

#endif