#ifndef __H_MESH__
#define __H_MESH__

#include <string>
#include <vector>
#include <map>

#include "yellowEngine/Utility/Definitions.hpp"
#include "yellowEngine/Math/AABB.hpp"
#include "yellowEngine/Math/Vector2.hpp"
#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Rendering/VertexLayout.hpp"


namespace yellowEngine
{
	class Mesh
	{
		friend class Model;

	public:
		struct Vertex
		{
			Vector3 position;
			Vector3 normal;
			Vector2 uv;

			bool operator<(const Vertex& vertex) const;
		};

		static Mesh* create(const char* path);

		unsigned int getVertexCount() const;
		unsigned int getVertexBufferHandle() const;
		unsigned int getElementBufferHandle() const;
		const VertexLayout& getVertexLayout() const;
		const AABB& getBounds() const;
		std::string getPath();

	private:
		static Mesh* loadOBJ(const char* path);
		Mesh(const char* path, const VertexLayout& vertexLayout,
			int vertexCount, void* vertexData,
			int indexCount, void* indexData);
		~Mesh();

		static std::map<std::string, Mesh*> __meshCache;

		std::string _path;
		unsigned int _vertexBufferHandle;
		unsigned int _elementBufferHandle;
		unsigned int _vertexCount;
		const VertexLayout _vertexLayout;
		AABB _bounds;
	};
}

#endif