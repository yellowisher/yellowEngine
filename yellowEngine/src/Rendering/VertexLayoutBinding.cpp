#include <glad/glad.h>

#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"


namespace yellowEngine
{
	std::map<std::pair<Mesh*, Shader*>, VertexLayoutBinding*> VertexLayoutBinding::__bindingCache;


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
		auto key = std::make_pair(mesh, shader);
		auto it = __bindingCache.find(key);
		if (it != __bindingCache.end())
		{
			return it->second;
		}

		auto binding = new VertexLayoutBinding();

		glGenVertexArrays(1, &binding->_vertexArrayHandle);
		glBindVertexArray(binding->_vertexArrayHandle);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getElementBufferHandle());

		auto attributes = shader->getAttributes();
		const VertexLayout& meshLayout = mesh->getVertexLayout();

		for (auto attribute : attributes)
		{
			auto meshAttr = meshLayout.getAttr(attribute.name);
			glVertexAttribPointer(attribute.location, meshAttr.size, meshAttr.type, GL_FALSE, meshLayout.getVertexSize(), (void*)meshAttr.offset);
			glEnableVertexAttribArray(attribute.location);
		}

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);

		__bindingCache.insert({ key,binding });

		return binding;
	}


	void VertexLayoutBinding::bind()
	{
		glBindVertexArray(_vertexArrayHandle);
	}


	void VertexLayoutBinding::unbind()
	{
		glBindVertexArray(NULL);
	}
}