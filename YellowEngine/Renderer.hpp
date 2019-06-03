#ifndef __H_RENDERER__
#define __H_RENDERER__

#include <vector>

#include "AABB.hpp"
#include "Vector3.hpp"
#include "ShaderProgram.hpp"
#include "VertexLayout.hpp"

// non-component generic renderer

class Renderer
{
public:
	Renderer();
	Renderer(VertexLayout& layout, ShaderProgram* shader);
	~Renderer();

	void setData(const AABB& aabb);
	void bufferData();
	void render();

	std::vector<Vector3> lines;

private:
	unsigned int _vbo;
	unsigned int _vao;

	ShaderProgram* _shader;
};

#endif