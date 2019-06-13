#ifndef __H_VERTEXLAYOUTBINDING__
#define __H_VERTEXLAYOUTBINDING__

#include <vector>

#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/Shader.hpp"

namespace yellowEngine
{
	class VertexLayoutBinding
	{
	public:
		static VertexLayoutBinding* create(Mesh* mesh, Shader* shader);

		unsigned int getVertexArrayHandle();

	private:
		static std::vector<VertexLayoutBinding*> __bindingCache;

		unsigned int _vertexArrayHandle;
		Mesh* _mesh;
		Shader* _shader;

		VertexLayoutBinding();
		~VertexLayoutBinding();
	};
}

#endif