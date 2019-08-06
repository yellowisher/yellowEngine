#include "CameraScript.hpp"
#include "Building.hpp"

using namespace yellowEngine;

COMPONENT_IMPL(Building)


void Building::start()
{
	_dead = false;
	_castle = transform->findChild("Castle");
	_ruin = transform->findChild("Destroy");
	if (_ruin)
	{
		//_ruin->gameObject->setActive(false);
	}
}


void Building::die()
{
	if (!_dead)
	{
		_dead = true;
		if (_castle)
		{
			CameraScript::win();
			delete(_castle->gameObject);
		}
		if (_ruin)
		{
			_ruin->gameObject->setActive(true);
		}
	}
}