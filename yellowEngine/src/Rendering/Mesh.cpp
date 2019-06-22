using namespace std;

#include <iostream>
#include <fstream>
#include <map>
#include <glad/glad.h>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/System/Game.hpp"
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


	Mesh::Mesh(const VertexLayout& vertexLayout,
		int vertexCount, void* vertexData,
		int indexCount, void* indexData) :
		_vertexLayout(vertexLayout)
	{
		_vertexCount = indexCount;

		glGenBuffers(1, &_vertexBufferHandle);
		glGenBuffers(1, &_elementBufferHandle);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferHandle);

		glBufferData(GL_ARRAY_BUFFER, vertexLayout.getVertexSize() * vertexCount, vertexData, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		_bounds.max = Vector3(-Utils::inf, -Utils::inf, -Utils::inf);
		_bounds.min = Vector3(Utils::inf, Utils::inf, Utils::inf);

		// naively assume first 3 elements are position values
		int offset = vertexLayout.getVertexSize() / sizeof(float);
		float* cursor = (float*)vertexData;
		for (int i = 0; i < vertexCount; i++)
		{
			_bounds.max.x = Utils::max(_bounds.max.x, *cursor);
			_bounds.max.y = Utils::max(_bounds.max.y, *(cursor + 1));
			_bounds.max.z = Utils::max(_bounds.max.z, *(cursor + 2));

			_bounds.min.x = Utils::min(_bounds.min.x, *cursor);
			_bounds.min.y = Utils::min(_bounds.min.y, *(cursor + 1));
			_bounds.min.z = Utils::min(_bounds.min.z, *(cursor + 2));
			cursor += offset;
		}
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

		Mesh* mesh = nullptr;

		mesh = loadOBJ(path);
		if (mesh != nullptr)__meshCache.insert({ path, mesh });
		return mesh;
	}


	Mesh* Mesh::loadOBJ(const char* path)
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

		ifstream fin;
		char input, c;

		std::string pathString = Game::getAssetPath(path);
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
						Vertex vertex = { positions[pi[i] - 1], normals[ni[i] - 1], uvs[uvi[i] - 1] };

						auto it = uniqueVertices.find(vertex);
						if (it == uniqueVertices.end())
						{
							index = (int)uniqueVertices.size();
							vertices.push_back(vertex);
							verticesIndex.push_back(index);
							uniqueVertices.insert({ vertex, index });
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
			Attr_Position,
			Attr_Normal,
			Attr_TexCoord0
			});

		Mesh* mesh = new Mesh(layout, (int)vertices.size(), &vertices[0], (int)verticesIndex.size(), &verticesIndex[0]);
		return mesh;
	}


	unsigned int Mesh::getVertexCount() const
	{
		return _vertexCount;
	}
}