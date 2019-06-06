#ifndef __H_VERTEXLAYOUTBINDING__
#define __H_VERTEXLAYOUTBINDING__

#include <vector>

#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/ShaderProgram.hpp"

namespace yellowEngine
{
	class VertexLayoutBinding
	{
	public:
		static VertexLayoutBinding* create(Mesh* mesh, ShaderProgram* shader);

		unsigned int getVertexArrayHandle();

	private:
		static std::vector<VertexLayoutBinding*> __bindingCache;

		unsigned int _vertexArrayHandle;
		Mesh* _mesh;
		ShaderProgram* _shader;

		VertexLayoutBinding();
		~VertexLayoutBinding();
		void bind(Mesh* mesh, ShaderProgram* shader);
	};
}

#endif