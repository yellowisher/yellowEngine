#include <glad/glad.h>

#include "yellowEngine/Rendering/Renderer.hpp"


Renderer::Renderer()
{
}


Renderer::Renderer(VertexLayout& layout, ShaderProgram* shader) :_shader(shader)
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	layout.bind();

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}


Renderer::~Renderer()
{
}


void Renderer::setData(const AABB& aabb)
{
	// just calculating rendering points... I'm sorry if you tried to understand this
	for (int i = 0; i < 4; i++)
	{
		int p[3] = { i >> 1, i & 1, (i == 1 || i == 2) };

		Vector3 begin = Vector3(aabb[p[0]].x, aabb[p[1]].y, aabb[p[2]].z);

		for (int b = 0; b < 3; b++)
		{
			Vector3 end = begin;
			end.v[b] = aabb[1 - p[b]].v[b];

			lines.push_back(begin);
			lines.push_back(end);
		}
	}

	bufferData();
}


void Renderer::bufferData()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * lines.size(), &lines[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}


void Renderer::render()
{
	glLineWidth(2);
	_shader->use();
	_shader->updateUniforms(nullptr);

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINES, 0, (GLsizei)lines.size());
	glBindVertexArray(NULL);
}