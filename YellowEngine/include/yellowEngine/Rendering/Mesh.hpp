#ifndef __H_MESH__
#define __H_MESH__

#include <string>
#include <vector>
#include <map>

#include "yellowEngine/Math/Vector2.hpp"
#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Rendering/VertexLayout.hpp"

namespace yellowEngine
{
	class Mesh
	{
	public:
		struct Vertex
		{
			Vector3 position;
			Vector3 normal;
			Vector2 uv;

			bool operator<(const Vertex& vertex) const;
		};

		struct Bounds
		{
			Vector3 min;
			Vector3 max;
		};

		static Mesh* create(const char* path);
		static Mesh* create(const VertexLayout& layout);

		unsigned int getVertexCount() const;
		unsigned int getVertexBufferHandle() const;
		unsigned int getElementBufferHandle() const;
		const VertexLayout& getVertexLayout() const;
		const Bounds& getBounds() const;

	private:
		static std::map<std::string, Mesh*> __meshCache;

		unsigned int _vertexBufferHandle;
		unsigned int _elementBufferHandle;
		unsigned int _vertexCount;
		const VertexLayout _vertexLayout;
		Bounds _bounds;

		static Mesh* createFromOBJ(const char* path);

		Mesh(const VertexLayout& vertexLayout);
		~Mesh();
	};
}

#endif