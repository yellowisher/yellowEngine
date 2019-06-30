#include <glad/glad.h>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Component/Light.hpp"
#include "yellowEngine/Component/Transform.hpp"


namespace yellowEngine
{
	COMPONENT_IMPL(Light)
	std::vector<Light*> Light::__lights[Num_LightType];

	const std::vector<Light*>& Light::getLights(LightType type)
	{
		return __lights[type];
	}


	Light::Light(GameObject* gameObject) :Component(gameObject)
	{
		color = Vector3(1.0f, 1.0f, 1.0f);
		ambientIntensity = 0.1f;
		diffuseIntensity = 1.0f;

		setCutoff(12.5f, 17.5f);

		constant = 1.0f;
		linear = 0.14f;
		quadratic = 0.07f;

		_type = LightType_Dir;
		__lights[LightType_Dir].push_back(this);
	}


	Light::~Light()
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
	}


	Light* Light::setType(LightType type)
	{
		bool found = false;
		for (int lt = 0; lt < Num_LightType; lt++)
		{
			for (int i = 0; i < __lights[lt].size(); i++)
			{
				if (__lights[lt][i] == this)
				{
					__lights[lt][i] = __lights[lt].back();
					__lights[lt].pop_back();
					found = true;
					break;
				}
			}
			if (found) break;
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


	float Light::getCutoffCos() const
	{
		return _cutoffCos;
	}


	float Light::getOuterCutoffCos() const
	{
		return _outerCutoffCos;
	}


	Vector3 Light::getDirection() const
	{
		return transform->getForward();
	}


	void Light::onValueChanged()
	{
		setType(_type);
		if (_type == LightType_Spot) setCutoff(_cutoffDegree, _outerCutoffDegree);
	}
}