#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Component/GameObject.hpp"


namespace yellowEngine
{
	Component* Component::createComponent(const std::string& type, GameObject* gameObject)
	{
		return getConstructors()[type](gameObject);
	}


	// Construct On First Use Idiom
	// https://isocpp.org/wiki/faq/ctors#static-init-order
	std::map<std::string, Component*(*)(GameObject*)>& Component::getConstructors()
	{
		static auto constructors = new std::map<std::string, Component*(*)(GameObject*)>();
		return *constructors;
	}


	std::vector<std::string>& Component::getComponents()
	{
		static auto components = new std::vector<std::string>();
		return *components;
	}


	std::map<std::string, std::vector<Component::Property>>& Component::getProperties()
	{
		static auto properties = new std::map<std::string, std::vector<Component::Property>>();
		return *properties;
	}


	Component::Component(GameObject* gameObject) :gameObject(gameObject), transform(gameObject->transform)
	{
		_active = true;
	}


	Component::~Component()
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