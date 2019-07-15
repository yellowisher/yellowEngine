#ifndef __H_LIGHT__
#define __H_LIGHT__

#include <vector>

#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Rendering/FrameBuffer.hpp"
#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Math/Matrix.hpp"
#include "yellowEngine/Math/Vector3.hpp"

namespace yellowEngine
{
	class Light : public Component
	{
		BEGIN_COMPONENT(Light)
			PROPERTY(Light, LightType, _type, "Type")
			PROPERTY(Light, Vector3, color, "Color")
			PROPERTY(Light, bool, castShadow, "Cast Shadow");
			PROPERTY(Light, float, ambientIntensity, "Ambient Intensity")
			PROPERTY(Light, float, diffuseIntensity, "Diffuse Intensity")
			PROPERTY(Light, float, _cutoffDegree, "Cutoff Degree")
			PROPERTY(Light, float, _outerCutoffDegree, "Outer Cutoff Degree")

			BEGIN_ENUM(Light, LightType)
				ENUM("Directional")
				ENUM("Point")
				ENUM("Spot")
			END_ENUM
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
		virtual void onValueChanged();

		Light* setType(LightType type);
		Light::LightType getType() { return _type; }
		void setCutoff(float inner, float outer);
		float getCutoffCos() const;
		float getOuterCutoffCos() const;
		Vector3 getDirection() const;
		float getRange();

		// TODO: use dirty bit pattern
		Matrix getProjMatrix();
		Matrix getProjViewMatrix();
		FrameBuffer* getShadowBuffer();
		float getZnear() { return _zNear; }
		float getZfar() { return _zFar; }
		void updateUniforms(Shader* shader);

		bool castShadow;
		Vector3 color;
		float ambientIntensity;
		float diffuseIntensity;

		float constant;
		float linear;
		float quadratic;

	private:
		static std::vector<Light*> __lights[Num_LightType];

		LightType _type;

		float _cutoffDegree;
		float _outerCutoffDegree;

		float _cutoffCos;
		float _outerCutoffCos;

		Matrix _projectionMatrix;
		FrameBuffer* _shadowBuffer;
		float _zNear;
		float _zFar;

		bool valueChanged;
	};
}

#endif