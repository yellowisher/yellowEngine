#ifndef __H_SPAWNER__
#define __H_SPAWNER__

#include "yellowEngine/yellowEngine.hpp"

using namespace yellowEngine;

class Spawner : public Component
{
public:
	Spawner(GameObject* gameObject) :Component(gameObject) {};
	~Spawner();

	void Spawner();
};

#endif