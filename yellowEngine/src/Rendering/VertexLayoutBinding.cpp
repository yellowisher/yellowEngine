#include <glad/glad.h>

#include "yellowEngine/Utility/Utils.hpp"
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

		Utils::printGLError("Begin vertex layout binding");

		glGenVertexArrays(1, &binding->_vertexArrayHandle);
		glBindVertexArray(binding->_vertexArrayHandle);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getElementBufferHandle());

		auto shaderAttrs = shader->getAttributes();
		auto meshAttrs = mesh->getVertexLayout().getAttributes();

		for (auto shaderAttr : shaderAttrs)
		{
			auto it = meshAttrs.find(shaderAttr.name);
			if (it != meshAttrs.end())
			{
				auto& meshAttr = it->second;
				glVertexAttribPointer(shaderAttr.location, it->second.size, meshAttr.type, GL_FALSE, mesh->getVertexLayout().getVertexSize(), (void*)meshAttr.offset);
				glEnableVertexAttribArray(shaderAttr.location);
			}
			else
			{
				glDisableVertexAttribArray(shaderAttr.location);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);

		Utils::printGLError("End vertex layout binding");

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