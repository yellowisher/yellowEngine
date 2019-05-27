#ifndef __H_LIGHT__
#define __H_LIGHT__

#include <vector>

#include "Component.hpp"
#include "Vector3.hpp"

class Light : public Component
{
public:
	enum Type
	{
		Type_None = -1,
		Type_Directional,
		Type_Point,
		Type_Spot,
		Num_Type,
	};

	static void updateUniformBuffer();

	static std::vector<Light*> lights[Num_Type];

	Light(GameObject* gameObject);
	~Light();

	Light* setDirectional();
	Light* setPoint(float constant, float linear, float quadratic);
	Light* setSpot(float constant, float linear, float quadratic, float cutoffCos, float outerCutoffCos);

	void setColor(Vector3 ambient, Vector3 diffuse, Vector3 specular);
	void setDirection(float x, float y, float z);

private:
	static float* addPadding(const Vector3& vector, float* arr);

	static unsigned int __ubo;

	Type _type;
	Vector3 _ambientColor;
	Vector3 _diffuseColor;
	Vector3 _specularColor;

	float _cutoffCos;
	float _outerCutoffCos;

	float _constant;
	float _linear;
	float _quadratic;

	void setType(Type type);
};

#endif