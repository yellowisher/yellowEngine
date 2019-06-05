#include <glad/glad.h>

#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"


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
	if (mesh->getElementBufferHandle() != -1)glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getElementBufferHandle());

	mesh->getVertexLayout().bind();

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}