#include <glad/glad.h>

#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"


namespace yellowEngine
{
	std::vector<VertexLayoutBinding*> VertexLayoutBinding::__bindingCache;


	VertexLayoutBinding::VertexLayoutBinding()
	{
	}


	VertexLayoutBinding::~VertexLayoutBinding()
	{
	}


	unsigned int VertexLayoutBinding::getVertexArrayHandle()
	{
		return _vertexArrayHandle;
	}


	VertexLayoutBinding* VertexLayoutBinding::create(Mesh* mesh, Shader* shader)
	{
		for (auto _binding : __bindingCache)
		{
			if (_binding->_mesh == mesh && _binding->_shader == shader)
			{
				return _binding;
			}
		}

		auto binding = new VertexLayoutBinding();
		binding->_mesh = mesh;
		binding->_shader = shader;

		glGenVertexArrays(1, &binding->_vertexArrayHandle);
		glBindVertexArray(binding->_vertexArrayHandle);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getElementBufferHandle());

		auto attributes = shader->getAttributes();
		const VertexLayout& meshLayout = mesh->getVertexLayout();

		for (int i = 0; i < attributes.size(); i++)
		{
			auto attr = meshLayout.getAttr(attributes[i].name);
			glVertexAttribPointer(i, attr.size, attr.type, GL_FALSE, meshLayout.getVertexSize(), (void*)attr.offset);
			glEnableVertexAttribArray(i);
		}

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);

		__bindingCache.push_back(binding);

		return binding;
	}
}