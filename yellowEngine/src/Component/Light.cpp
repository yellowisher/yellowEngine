#include <glad/glad.h>

#include "yellowEngine/Utility/Utils.hpp"
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
		ambiendIntensity = 0.1f;
		diffuseIntensity = 1.0f;

		setCutoff(25.0f, 27.5f);

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


	void Light::setCutoff(float inner, float outer)
	{
		_cutoffDegree = inner;
		_outerCutoffDegree = outer;

		_cutoffCos = cosf(Utils::deg2rad(_cutoffDegree));
		_outerCutoffCos = cosf(Utils::deg2rad(_outerCutoffDegree));
	}


	float Light::getCutoffCos()
	{
		return _cutoffCos;
	}


	float Light::getOuterCutoffCos()
	{
		return _outerCutoffCos;
	}


	Vector3 Light::getDirection()
	{
		return transform->getForward();
	}
}