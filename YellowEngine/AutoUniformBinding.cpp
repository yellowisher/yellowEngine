#include "Renderer.hpp"
#include "GameObject.hpp"
#include "ShaderProgram.hpp"
#include "AutoUniformBinding.hpp"


AutoUniformBinding::StaticConstructor AutoUniformBinding::__staticConstructor;
const char* AutoUniformBinding::__uniformStrings[Num_Uniforms];


AutoUniformBinding::AutoUniformBinding(ShaderProgram* shader) :_shader(shader)
{
}


AutoUniformBinding::~AutoUniformBinding()
{
}


void AutoUniformBinding::initialize()
{
	for (int i = 0; i < Num_Uniforms; i++)
	{
		const Uniform* uniform = _shader->getUniform(__uniformStrings[i]);
		if (uniform != nullptr)
		{
			_uniformPairs.push_back({ (UniformType)i, uniform });
		}
	}
}


void AutoUniformBinding::bind(GameObject* user) const
{
	for (auto pair : _uniformPairs)
	{
		switch (pair.type)
		{
			case Uniform_Model:
				if (user->transform->matrixChanged())
				{
					_shader->setUniform(pair.uniform, user->transform->getMatrix(true));
				}
				break;
			case Uniform_ProjectionView:
				if (Renderer::getCurrentCamera()->matrixChanged())
				{
					_shader->setUniform(pair.uniform, Renderer::getCurrentCamera()->getMatrix());
				}
				break;
			case Uniform_ModelColor:
				break;
			case Uniform_LightColor:
				break;
		}
	}
}