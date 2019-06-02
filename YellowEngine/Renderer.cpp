#include <gl/glew.h>

#include "Renderer.hpp"


Renderer::Renderer()
{
}


Renderer::Renderer(VertexLayout& layout, ShaderProgram* shader, GameObject* gameObject):
	_shader(shader), _gameObject(gameObject)
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


void Renderer::setData(Box&& box)
{
	_type = Type_Box;
	Vector3 vertices[Box::count];
	vertices[0] = Vector3(box.min.x, box.min.y, box.min.z);
	vertices[0] = Vector3(box.min.x, box.min.y, box.min.z);

	// just calculating rendering points... I'm sorry if you tried to understand this
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		int p[3] = { i >> 1, i & 1, (i == 1 || i == 2) };

		Vector3 origin = Vector3(box.m[p[0]].x, box.m[p[1]].y, box.m[p[2]].z);

		for (int b = 0; b < 3; b++)
		{
			Vector3 other = origin;
			other.v[b] = box.m[1 - p[b]].v[b];

			vertices[count++] = origin;
			vertices[count++] = other;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * Box::count, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}


void Renderer::setData(AABB aabb)
{
	setData(Box(aabb.min(), aabb.max()));
}


void Renderer::setData(Sphere& sphere)
{
	_type = Type_Sphere;

}


void Renderer::render()
{
	glLineWidth(2);
	_shader->use();
	_shader->updateUniforms(_gameObject);

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINES, 0, getCount(_type));
	glBindVertexArray(NULL);
}