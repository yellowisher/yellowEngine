#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/System/GameObject.hpp"


namespace yellowEngine
{
	Component::Component(GameObject* gameObject) :gameObject(gameObject), transform(gameObject->transform)
	{
		_active = true;
	}


	Component::~Component()
	{
	}


	void Component::onCreate()
	{
	}


	void Component::onDestroy()
	{
	}


	void Component::setActive(bool active)
	{
		_active = active;
	}


	bool Component::getActive()
	{
		return _active;
	}
}