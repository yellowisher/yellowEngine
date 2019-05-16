#include <GL/glew.h>

#include "VertexLayoutBinding.hpp"


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


VertexLayoutBinding* VertexLayoutBinding::create(Mesh* mesh, ShaderProgram* shader)
{
	for (auto _binding : __bindingCache)
	{
		if (_binding->_mesh == mesh && _binding->_shader == shader)
		{
			return _binding;
		}
	}

	auto binding = new VertexLayoutBinding();
	binding->bind(mesh, shader);
	__bindingCache.push_back(binding);

	return binding;
}


void VertexLayoutBinding::bind(Mesh* mesh, ShaderProgram* shader)
{
	_mesh = mesh;
	_shader = shader;

	glGenVertexArrays(1, &_vertexArrayHandle);
	glBindVertexArray(_vertexArrayHandle);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferHandle());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getElementBufferHandle());

	VertexLayout layout = mesh->getVertexLayout();
	unsigned int offset = 0;
	for (int i = 0; i < layout.getAttrCount(); i++)
	{
		auto attr = layout.getAttr(i);
		glVertexAttribPointer(i, attr.size, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offset);
		offset += sizeof(float)*attr.size;
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glBindVertexArray(NULL);
}