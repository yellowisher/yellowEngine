using namespace std;

#include <iostream>
#include <fstream>
#include <map>
#include <glad/glad.h>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/System/System.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"


namespace yellowEngine
{
	map<string, Mesh*> Mesh::__meshCache;


	bool Mesh::Vertex::operator<(const Vertex& vertex) const
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


	Mesh::Mesh(const VertexLayout& vertexLayout) :
		_vertexLayout(vertexLayout),
		_vertexBufferHandle(-1),
		_elementBufferHandle(-1)
	{
	}


	Mesh::~Mesh()
	{
	}


	unsigned int Mesh::getVertexBufferHandle() const
	{
		return _vertexBufferHandle;
	}


	unsigned int Mesh::getElementBufferHandle() const
	{
		return _elementBufferHandle;
	}


	const VertexLayout& Mesh::getVertexLayout() const
	{
		return _vertexLayout;
	}


	const Mesh::Bounds& Mesh::getBounds() const
	{
		return _bounds;
	}


	Mesh* Mesh::create(const char* path)
	{
		auto it = __meshCache.find(path);
		if (it != __meshCache.end())
		{
			return it->second;
		}

		Mesh* mesh = createFromOBJ(path);
		if (mesh != nullptr)__meshCache.insert({ path, mesh });
		return mesh;
	}


	Mesh* Mesh::create(const VertexLayout& layout)
	{
		Mesh* mesh = new Mesh(layout);
		glGenBuffers(1, &mesh->_vertexBufferHandle);
		return mesh;
	}


	Mesh* Mesh::createFromOBJ(const char* path)
	{
		int index;
		vector<Vertex> vertices;
		vector<unsigned int> verticesIndex;
		map<Vertex, unsigned int> uniqueVertices;

		vector<Vector3> positions, normals;
		vector<Vector2> uvs;

		Vector3 vec3;
		Vector2 vec2;
		unsigned int pi[3], ni[3], uvi[3];

		Vector3 max = Vector3(-Utils::inf, -Utils::inf, -Utils::inf);
		Vector3 min = Vector3(Utils::inf, Utils::inf, Utils::inf);

		ifstream fin;
		char input, c;

		std::string pathString = System::getInstance()->getResourcePath(path).c_str();
		fin.open(pathString);

		if (fin.fail())
		{
			cout << "Cannot open file " << pathString << endl;
			return nullptr;
		}

		fin.get(input);
		while (!fin.eof())
		{
			if (input == 'v')
			{
				fin.get(input);
				if (input == ' ')
				{
					fin >> vec3.x >> vec3.y >> vec3.z;
					vec3.z *= -1.0f;
					positions.push_back(vec3);
				}
				else if (input == 'n')
				{
					fin >> vec3.x >> vec3.y >> vec3.z;
					vec3.z *= -1.0f;
					normals.push_back(vec3);
				}
				else if (input == 't')
				{
					fin >> vec2.x >> vec2.y;
					vec2.y = 1.0f - vec2.y;
					uvs.push_back(vec2);
				}
			}
			else if (input == 'f')
			{
				fin.get(input);
				if (input == ' ')
				{
					fin >> pi[2] >> c >> uvi[2] >> c >> ni[2]
						>> pi[1] >> c >> uvi[1] >> c >> ni[1]
						>> pi[0] >> c >> uvi[0] >> c >> ni[0];

					for (int i = 0; i < 3; i++)
					{
						Vertex vertex = { positions[pi[i] - 1], normals[ni[i] - 1],uvs[uvi[i] - 1] };
						auto it = uniqueVertices.find(vertex);
						if (it == uniqueVertices.end())
						{
							index = (int)uniqueVertices.size();
							vertices.push_back(vertex);
							verticesIndex.push_back(index);
							uniqueVertices.insert({ vertex, index });

							max.x = Utils::max(max.x, vertex.position.x);
							max.y = Utils::max(max.y, vertex.position.y);
							max.z = Utils::max(max.z, vertex.position.z);

							min.x = Utils::min(min.x, vertex.position.x);
							min.y = Utils::min(min.y, vertex.position.y);
							min.z = Utils::min(min.z, vertex.position.z);
						}
						else
						{
							index = it->second;
							verticesIndex.push_back(index);
						}
					}
				}
			}
			while (input != '\n')
			{
				fin.get(input);
			}
			if (fin.eof())
			{
				break;
			}
			fin.get(input);
		}
		fin.close();

		VertexLayout layout({
			VertexLayout::Attribute(VertexLayout::Attr_Position,3),
			VertexLayout::Attribute(VertexLayout::Attr_Normal,3),
			VertexLayout::Attribute(VertexLayout::Attr_TexCoord0,2)
			});
		Mesh* mesh = new Mesh(layout);
		mesh->_vertexCount = (unsigned int)verticesIndex.size();
		mesh->_bounds.max = max;
		mesh->_bounds.min = min;

		glGenBuffers(1, &mesh->_vertexBufferHandle);
		glGenBuffers(1, &mesh->_elementBufferHandle);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->_vertexBufferHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->_elementBufferHandle);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * verticesIndex.size(), &verticesIndex[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		return mesh;
	}


	unsigned int Mesh::getVertexCount() const
	{
		return _vertexCount;
	}
}