#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Component/GameObject.hpp"


namespace yellowEngine
{
	std::map <std::string, Component*(*)(GameObject*)> Component::__constructors;


	Component* Component::createComponent(const std::string& type, GameObject* gameObject)
	{
		return __constructors[type](gameObject);
	}


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