#ifndef __H_LIGHT__
#define __H_LIGHT__

#include <vector>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Math/Vector3.hpp"

namespace yellowEngine
{
	class Light : public Component
	{
		BEGIN_COMPONENT(Light)
			PROPERTY(Light, LightType, _type, "Type")
			PROPERTY(Light, Vector3, color, "Color")
		END_COMPONENT

	public:
		enum LightType
		{
			LightType_Dir,
			LightType_Point,
			LightType_Spot,
			Num_LightType,
		};

		static const std::vector<Light*>& getLights(LightType type);

		Light(GameObject* gameObject);
		~Light();
		void onValueChanged();
		Light* setType(LightType type);
		void setCutoff(float inner, float outer);
		float getCutoffCos() const;
		float getOuterCutoffCos() const;

		Vector3 getDirection() const;

		Vector3 color;
		float ambiendIntensity;
		float diffuseIntensity;

		float constant;
		float linear;
		float quadratic;

	private:
		static std::vector<Light*> __lights[Num_LightType];

		LightType _type;
		// only for spot light
		float _cutoffDegree;
		float _outerCutoffDegree;

		float _cutoffCos;
		float _outerCutoffCos;
	};
}

#endif