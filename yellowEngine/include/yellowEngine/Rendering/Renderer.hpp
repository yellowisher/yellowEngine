#ifndef __H_RENDERER__
#define __H_RENDERER__

#include <vector>

#include "yellowEngine/Math/AABB.hpp"
#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Rendering/VertexLayout.hpp"

// non-component generic renderer

namespace yellowEngine
{
	class Renderer
	{
	public:
		Renderer();
		Renderer(VertexLayout& layout, Shader* shader);
		~Renderer();

		void setData(const AABB& aabb);
		void bufferData();
		void render();

		std::vector<Vector3> data;

	private:
		unsigned int _vbo;
		unsigned int _vao;

		Shader* _shader;
	};
}

#endif