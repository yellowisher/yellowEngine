#include "Renderer.hpp"
#include "GameObject.hpp"
#include "ShaderProgram.hpp"
#include "UniformUpdater.hpp"


UniformUpdater::StaticConstructor UniformUpdater::__staticConstructor;
const char* UniformUpdater::__uniformStrings[Num_Uniforms];


UniformUpdater::UniformUpdater(ShaderProgram* shader) :_shader(shader)
{
}


UniformUpdater::~UniformUpdater()
{
}


void UniformUpdater::initialize()
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


void UniformUpdater::update(GameObject* user) const
{
	for (auto pair : _uniformPairs)
	{
		switch (pair.type)
		{
			case Uniform_Model:
				if (!user->transform->matrixPulled())
				{
					_shader->setUniform(pair.uniform, user->transform->getMatrix(true));
				}
				break;

			case Uniform_ProjectionView:
				if (!Renderer::getCurrentCamera()->matrixPulled())
				{
					_shader->setUniform(pair.uniform, Renderer::getCurrentCamera()->getMatrix());
				}
				break;

			case Uniform_CameraPosition:
				_shader->setUniform(pair.uniform, Renderer::getCurrentCamera()->transform->getWorldPosition());
				break;
		}
	}
}