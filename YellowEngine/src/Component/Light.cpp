#include <glad/glad.h>

#include "yellowEngine/Component/Light.hpp"
#include "yellowEngine/Component/Transform.hpp"


std::vector<Light*> Light::lights[Num_Type];
unsigned int Light::__ubo;
bool Light::__initialized = false;


Light::Light(GameObject* gameObject) :
	Component(gameObject),
	_ambientColor(0.1f, 0.1f, 0.1f),
	_diffuseColor(0.5f, 0.5f, 0.5f),
	_specularColor(1.0f, 1.0f, 1.0f),
	_type(Type_None)
{
}


Light::~Light()
{
	if (_type != Type_None)
	{
		for (auto it = lights[_type].begin(); it != lights[_type].end(); ++it)
		{
			if (*it == this)
			{
				lights[_type].erase(it);
				break;
			}
		}
	}
}


void Light::setColor(Vector3 ambient, Vector3 diffuse, Vector3 specular)
{
	_ambientColor = ambient;
	_diffuseColor = diffuse;
	_specularColor = specular;
}


void Light::setType(Type type)
{
	if (_type != Type_None)
	{
		for (auto it = lights[_type].begin(); it != lights[_type].end(); ++it)
		{
			if (*it == this)
			{
				lights[_type].erase(it);
				break;
			}
		}
	}
	_type = type;
	if (_type != Type_None)
	{
		lights[_type].push_back(this);
	}
}


Light* Light::setDirectional()
{
	setType(Type_Directional);

	return this;
}


Light* Light::setPoint(float constant, float linear, float quadratic)
{
	setType(Type_Point);
	_constant = constant;
	_linear = linear;
	_quadratic = quadratic;

	return this;
}


Light* Light::setSpot(float constant, float linear, float quadratic, float cutoffCos, float outerCutoffCos)
{
	setType(Type_Spot);
	_constant = constant;
	_linear = linear;
	_quadratic = quadratic;

	_cutoffCos = cutoffCos;
	_outerCutoffCos = outerCutoffCos;

	return this;
}


void Light::setDirection(float x, float y, float z)
{
	transform->setRotation(x, y, z);
}


void Light::updateUniformBuffer()
{
	if (!__initialized)
	{
		__initialized = true;
		glGenBuffers(1, &__ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, __ubo);
		glBufferData(GL_UNIFORM_BUFFER, 1680, NULL, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, __ubo);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, __ubo);

	int offset = 0;
	float temp[4] = { 0,0,0,0 };
	for (auto light : lights[Type_Directional])
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->transform->getWorldRotation(), temp)); offset += 16;

		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_ambientColor, temp)); offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_diffuseColor, temp)); offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_specularColor, temp)); offset += 16;
	}
	offset = 256;

	for (auto light : lights[Type_Point])
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->transform->getWorldPosition(), temp)); offset += 16;

		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_ambientColor, temp)); offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_diffuseColor, temp)); offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_specularColor, temp)); offset += 16;

		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &light->_constant); offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &light->_linear); offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &light->_quadratic); offset += 4;
		offset += 4;

	}
	offset = 1216;

	for (auto light : lights[Type_Spot])
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->transform->getWorldPosition(), temp)); offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->transform->getWorldRotation(), temp)); offset += 16;

		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_ambientColor, temp)); offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_diffuseColor, temp)); offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, addPadding(light->_specularColor, temp)); offset += 16;

		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &light->_constant); offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &light->_linear); offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &light->_quadratic); offset += 4;

		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &light->_cutoffCos); offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &light->_outerCutoffCos); offset += 4;
		offset += 12;
	}
	offset = 1664;

	for (int i = 0; i < Num_Type; i++)
	{
		size_t count = lights[i].size();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, 4, &count); offset += 4;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}


float* Light::addPadding(const Vector3& vector, float* arr)
{
	arr[0] = vector.x;
	arr[1] = vector.y;
	arr[2] = vector.z;
	return arr;
}