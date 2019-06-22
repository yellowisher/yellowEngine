#include <glad/glad.h>

#include "yellowEngine/Rendering/Renderer.hpp"


namespace yellowEngine
{
	Renderer::Renderer()
	{
	}


	Renderer::Renderer(VertexLayout& layout, Shader* shader) :_shader(shader)
	{
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		auto attributes = shader->getAttributes();
		for (int i = 0; i < attributes.size(); i++)
		{
			auto attr = layout.getAttr(attributes[i].name);
			glVertexAttribPointer(i, attr.size, attr.type, GL_FALSE, layout.getVertexSize(), (void*)attr.offset);
			glEnableVertexAttribArray(i);
		}

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

				data.push_back(begin);
				data.push_back(end);
			}
		}

		bufferData();
	}


	void Renderer::bufferData()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * data.size(), &data[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}


	void Renderer::render()
	{
		glLineWidth(2);
		_shader->bind();
		_shader->updateUniforms(nullptr);

		glBindVertexArray(_vao);
		glDrawArrays(GL_LINES, 0, (GLsizei)data.size());
		glBindVertexArray(NULL);

		_shader->unbind();
	}
}