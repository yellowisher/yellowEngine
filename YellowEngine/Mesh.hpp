#ifndef __H_MESH__
#define __H_MESH__

using namespace std;

#include <string>
#include <vector>
#include <map>

#include "Vector2.hpp"
#include "Vector3.hpp"

class Mesh
{
public:
	static Mesh* create(const char* path);

	unsigned int getVertexArrayHandle() const;
	unsigned int getVertexCount() const;
	unsigned int getElementCount() const;

private:
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 uv;

		bool operator<(const Vertex& vertex) const
		{
			if (position == vertex.position)
			{
				if (normal == vertex.normal)
				{
					return uv < vertex.uv;
				}
				return normal < vertex.normal;
			}
			return position < vertex.position;
		}
	};

	static map<string, Mesh*> meshes;

	unsigned int vertexBufferHandle;
	unsigned int vertexArrayHandle;
	unsigned int elementBufferHandle;
	unsigned int vertexCount;
	unsigned int elementCount;

	static Mesh* createFromOBJ(const char* path);

	Mesh();
	~Mesh();
};

#endif