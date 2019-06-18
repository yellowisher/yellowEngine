#include <glad/glad.h>

#include "yellowEngine/Component/Light.hpp"
#include "yellowEngine/Component/Transform.hpp"


namespace yellowEngine
{
	std::vector<Light*> Light::__lights[Num_LightType];


	const std::vector<Light*>& Light::getLights(LightType type)
	{
		return __lights[type];
	}


	Light::Light(GameObject* gameObject) :Component(gameObject)
	{
		color = Vector3(1.0f, 1.0f, 1.0f);
		intensity = 1.0f;

		_type = LightType_Dir;
		__lights[LightType_Dir].push_back(this);
	}


	Light::~Light()
	{
	}


	Light* Light::setType(LightType type)
	{
		for (int i = 0; i < __lights[_type].size(); i++)
		{
			if (__lights[_type][i] == this)
			{
				__lights[_type][i] = __lights[_type].back();
				__lights[_type].pop_back();
				break;
			}
		}

		_type = type;
		__lights[_type].push_back(this);
		return this;
	}


	Vector3 Light::getDirection()
	{
		return transform->getForward();
	}
}