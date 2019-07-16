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

	std::map<std::string, std::map<std::string, std::vector<std::string>>>& Component::getEnums()
	{
		static auto enums = new std::map<std::string, std::map<std::string, std::vector<std::string>>>();
		return *enums;
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
		return gameObject->getActive() && _active;
	}


	void Component::clone(Component* target)
	{
		for (auto prop : getProperties()[getTypeName()])
		{
			if (prop.type == "Mesh" || prop.type == "Material")
			{
				std::string* dst = (std::string*)((size_t)target + prop.offset);
				std::string* src = (std::string*)((size_t)this + prop.offset);
				*dst = *src;
			}
			else
			{
				memcpy((void*)((size_t)target + prop.offset), (void*)((size_t)this + prop.offset), prop.size);
			}
		}
	}
}