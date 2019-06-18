#ifndef __H_VERTEXLAYOUTBINDING__
#define __H_VERTEXLAYOUTBINDING__

#include <vector>
#include <map>

#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/Shader.hpp"

namespace yellowEngine
{
	class VertexLayoutBinding
	{
	public:
		static VertexLayoutBinding* create(Mesh* mesh, Shader* shader);

		unsigned int getVertexArrayHandle();
		void bind();
		static void unbind();

	private:
		static std::map<std::pair<Mesh*, Shader*>, VertexLayoutBinding*> __bindingCache;

		VertexLayoutBinding();
		~VertexLayoutBinding();

		unsigned int _vertexArrayHandle;
	};
}

#endif