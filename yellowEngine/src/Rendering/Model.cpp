#include <queue>
#include <stack>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Utility/Definitions.hpp"
#include "yellowEngine/System/System.hpp"
#include "yellowEngine/Component/MeshRenderer.hpp"
#include "yellowEngine/Rendering/Model.hpp"

namespace yellowEngine
{
	std::map<std::string, Model*> Model::__modelCache;


	Model::Model()
	{
	}


	Model::~Model()
	{
		std::queue<Node*> visitQueue;
		visitQueue.push(_root);

		while (!visitQueue.empty())
		{
			Node* visit = visitQueue.front();
			visitQueue.pop();

			for (auto child : visit->children)
			{
				visitQueue.push(child);
			}
			delete(visit);
		}
	}


	GameObject* Model::instantiate(const char* name)
	{
		GameObject* rootObject = nullptr;
		std::queue<std::pair<Node*, Transform*>> visitQueue;
		visitQueue.push({ _root, nullptr });

		while (!visitQueue.empty())
		{
			auto visit = visitQueue.front();
			Node* node = visit.first;
			Transform* parent = visit.second;
			visitQueue.pop();

			GameObject* gameObject = new GameObject(node->name.c_str());
			// TODO: remove shader program
			if (node->mesh)gameObject->addComponent<MeshRenderer>()->set(node->mesh, Shader::create("Shader/texture.vert", "Shader/texture.frag"));

			if (parent != nullptr)
			{
				parent->addChild(gameObject->transform);
				gameObject->transform->setScale(node->scale);
				gameObject->transform->setRotation(node->rotation);
				gameObject->transform->setPosition(node->position);
			}
			else
			{
				rootObject = gameObject;
				rootObject->setName(name);
			}

			for (auto child : node->children)
			{
				visitQueue.push({ child, gameObject->transform });
			}
		}

		return rootObject;
	}


	Model* Model::create(const char* path)
	{
		auto it = __modelCache.find(path);
		if (it != __modelCache.end())
		{
			return it->second;
		}

		Model* model = loadFBX(path);
		__modelCache.insert({ path, model });
		return model;
	}


	Model* Model::loadFBX(const char* path)
	{
		std::string pathString = System::getInstance()->getResourcePath(path).c_str();
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(pathString, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Utils::print(importer.GetErrorString());
			return nullptr;
		}

		Model* model = new Model();
		model->_scene = scene;
		model->_root = model->buildTree(scene->mRootNode);

		// after build scene hierarchy, fill all the meshes
		model->fillMesh(model->_root);

		// free here
		return model;
	}


	void Model::fillMesh(Node* node)
	{
		if (node->aiMesh != nullptr)
		{
			node->mesh = createMesh(node->aiMesh);
		}

		for (auto child : node->children)
		{
			fillMesh(child);
		}
	}


	Model::Node* Model::buildTree(aiNode* aiNode)
	{
		Node* node = new Node();

		// TODO: figure it out in which situation a node contains multiple meshes
		// handle atmost 1 mesh
		if (aiNode->mNumMeshes > 0)
		{
			node->aiMesh = _scene->mMeshes[aiNode->mMeshes[0]];
		}
		else
		{
			node->aiMesh = nullptr;
		}

		Matrix matrix;
		for (int i = 0; i < 16; i++)
		{
			matrix[i] = aiNode->mTransformation[i / 4][i % 4];
		}
		node->position = matrix.extractTranslation();
		node->rotation = matrix.extractRotation();
		node->scale = matrix.extractScale();
		node->name = aiNode->mName.C_Str();
		_nodes.insert({ node->name, node });

		for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
		{
			Node* child = buildTree(aiNode->mChildren[i]);
			node->children.push_back(child);
		}

		return node;
	}


	Mesh* Model::createMesh(aiMesh* aiMesh)
	{	
		// Model class already caching model itself
		// there is no need to cache in Mesh class
		// so create directly rather than call Mesh::create
		Mesh* mesh = nullptr;

		// TODO: support vary vertex format
		// currently only handles model format of PNT(+B)
		if (aiMesh->HasBones())
		{
			std::vector<Mesh::SkinnedVertex> vertices(aiMesh->mNumVertices);
			std::vector<unsigned int> indices(aiMesh->mNumFaces * 3);

			for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
			{
				vertices[i].position.x = aiMesh->mVertices[i].x;
				vertices[i].position.y = aiMesh->mVertices[i].y;
				vertices[i].position.z = aiMesh->mVertices[i].z;

				vertices[i].normal.x = aiMesh->mNormals[i].x;
				vertices[i].normal.y = aiMesh->mNormals[i].y;
				vertices[i].normal.z = aiMesh->mNormals[i].z;

				vertices[i].uv.x = aiMesh->mTextureCoords[i]->x;
				vertices[i].uv.y = aiMesh->mTextureCoords[i]->y;

				for (int j = 0; j < MaxJointCount; j++)
				{
					vertices[i].joints[j] = NullJoint;
				}
			}

			for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
			{
				indices[i * 3]	   = aiMesh->mFaces[i].mIndices[0];
				indices[i * 3 + 1] = aiMesh->mFaces[i].mIndices[1];
				indices[i * 3 + 2] = aiMesh->mFaces[i].mIndices[2];
			}

			for (unsigned int b = 0; b < aiMesh->mNumBones; b++)
			{
				Node* joint = _nodes[aiMesh->mBones[b]->mName.C_Str()];
				joint->jointId = (float)b;
				for (unsigned int w = 0; w < aiMesh->mNumBones; w++)
				{
					int vi = aiMesh->mBones[b]->mWeights[w].mVertexId;

					int wi = 0;
					while (vertices[vi].joints[wi] != NullJoint) wi++;
					vertices[vi].joints[wi] = joint->jointId;
					vertices[vi].weights[wi] = aiMesh->mBones[b]->mWeights[w].mWeight;
				}
			}

			VertexLayout layout({
				Attr_Position,
				Attr_Normal,
				Attr_TexCoord0,
				Attr_Joints,
				Attr_Weights,
				});

			mesh = new Mesh(layout, (int)vertices.size(), &vertices[0], (int)indices.size(), &indices[0]);
		}
		else
		{
			std::vector<Mesh::Vertex> vertices(aiMesh->mNumVertices);
			std::vector<unsigned int> indices(aiMesh->mNumFaces * 3);

			for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
			{
				vertices[i].position.x = aiMesh->mVertices[i].x;
				vertices[i].position.y = aiMesh->mVertices[i].y;
				vertices[i].position.z = aiMesh->mVertices[i].z;

				vertices[i].normal.x = aiMesh->mNormals[i].x;
				vertices[i].normal.y = aiMesh->mNormals[i].y;
				vertices[i].normal.z = aiMesh->mNormals[i].z;

				vertices[i].uv.x = aiMesh->mTextureCoords[i]->x;
				vertices[i].uv.y = aiMesh->mTextureCoords[i]->y;
			}

			for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
			{
				indices[i * 3]	   = aiMesh->mFaces[i].mIndices[0];
				indices[i * 3 + 1] = aiMesh->mFaces[i].mIndices[1];
				indices[i * 3 + 2] = aiMesh->mFaces[i].mIndices[2];
			}

			VertexLayout layout({
				Attr_Position,
				Attr_Normal,
				Attr_TexCoord0,
				});

			mesh = new Mesh(layout, (int)vertices.size(), &vertices[0], (int)indices.size(), &indices[0]);
		}

		if (_scene->HasMaterials())
		{
			aiMaterial* material = _scene->mMaterials[aiMesh->mMaterialIndex];
			auto type = aiTextureType_DIFFUSE;
			for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
			{
				aiString path;
				material->GetTexture(type, i, &path);

				Texture::create(path.C_Str());
			}
		}

		return mesh;
	}
}