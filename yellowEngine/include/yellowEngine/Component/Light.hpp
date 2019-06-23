#ifndef __H_LIGHT__
#define __H_LIGHT__

#include <vector>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Math/Vector3.hpp"

namespace yellowEngine
{
	class Light : public Component
	{
	public:
		enum LightType
		{
			LightType_Dir,
			LightType_Point,
			LightType_Spot,
			Num_LightType,
		};

		static const std::vector<Light*>& getLights(LightType type);

		static constexpr float constant = 1.0f;
		static constexpr float linear = 0.14f;
		static constexpr float quadratic = 0.07;

		Light(GameObject* gameObject);
		~Light();
		Light* setType(LightType type);
		void setCutoff(float inner, float outer);
		float getCutoffCos();
		float getOuterCutoffCos();

		Vector3 getDirection();

		Vector3 color;
		float ambiendIntensity;
		float diffuseIntensity;

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