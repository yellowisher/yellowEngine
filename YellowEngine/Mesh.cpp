using namespace std;

#include <iostream>
#include <fstream>
#include <map>
#include <GL/glew.h>

#include "Mesh.hpp"

map<string, Mesh*> Mesh::meshes;

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}


Mesh* Mesh::create(const char* path)
{
	auto it = meshes.find(path);
	if (it != meshes.end())
	{
		return it->second;
	}

	Mesh* mesh = createFromOBJ(path);
	if (mesh != nullptr)meshes.insert({ path, mesh });
	return mesh;
}


Mesh* Mesh::createFromOBJ(const char* path)
{
	unsigned int index;
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

	fin.open(path);

	if (fin.fail())
	{
		cout << "Cannot open file " << path << endl;
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
						index = uniqueVertices.size();
						vertices.push_back(vertex);
						verticesIndex.push_back(index);
						uniqueVertices.insert({ vertex,index });
					}
					else
					{
						index = it->second;
						verticesIndex.push_back(index);
					}
				}
			}
		}
		while (input != '\n')fin.get(input);
		fin.get(input);
	}
	fin.close();

	Mesh* mesh = new Mesh();
	mesh->vertexCount = verticesIndex.size();
	mesh->elementCount = mesh->vertexCount / 3;

	glGenVertexArrays(1, &mesh->vertexArrayHandle);
	glGenBuffers(1, &mesh->vertexBufferHandle);
	glGenBuffers(1, &mesh->elementBufferHandle);

	glBindVertexArray(mesh->vertexArrayHandle);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->elementBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* verticesIndex.size(), &verticesIndex[0], GL_STATIC_DRAW);

	// TODO: do this in shader?
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(NULL);

	return mesh;
}


unsigned int Mesh::getVertexArrayHandle() const
{
	return vertexArrayHandle;
}


unsigned int Mesh::getVertexCount() const
{
	return vertexCount;
}


unsigned int Mesh::getElementCount() const
{
	return elementCount;
}