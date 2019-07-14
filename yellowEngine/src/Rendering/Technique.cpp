#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/Technique/Technique_Deferred.hpp"
#include "yellowEngine/Rendering/Technique.hpp"


namespace yellowEngine
{
	Technique* Technique::__techniques[Num_TechniqueType];


	Technique* Technique::getTechnique(TechniqueType type)
	{
		if (__techniques[type] == nullptr)
		{
			switch (type)
			{
				case TechniqueType_Deferred:
					__techniques[type] = new Technique_Deferred();
					break;
				default:
					break;
			}
		}

		return __techniques[type];
	}


	void Technique::renderScene(Camera* camera)
	{
		for (auto technique : __techniques)
		{
			if (technique)technique->_renderScene(camera);
		}
	}


	void Technique::addRenderer(ObjectRenderer* renderer)
	{
		_renderers.push_back(renderer);
	}


	void Technique::removeRenderer(ObjectRenderer* renderer)
	{
		for (auto it = _renderers.begin();; ++it)
		{
			if (*it == renderer)
			{
				*it = _renderers.back();
				_renderers.pop_back();
				break;
			}
		}
	}


	void Technique::renderQuad(Shader* shader)
	{
		static Mesh* quad = Mesh::create("./res/Mesh/quad.obj");
		VertexLayoutBinding::create(quad, shader)->bind();
		glDrawElements(GL_TRIANGLES, quad->getVertexCount(), GL_UNSIGNED_INT, 0);
	}
}