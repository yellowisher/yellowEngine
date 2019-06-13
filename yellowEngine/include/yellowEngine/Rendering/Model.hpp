#ifndef __H_MODEL__
#define __H_MODEL__

#include <map>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Math/Quaternion.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/System/GameObject.hpp"

namespace yellowEngine
{
	class Model
	{
		using Attribute = VertexLayout::Attribute;

	public:
		static Model* create(const char* path);
		
		GameObject* instantiate(const char* name);
		
	private:
		struct Node	
		{
			union
			{
				Mesh* mesh;
				aiMesh* aiMesh;
			};
			float jointId;

			Vector3 position;
			Vector3 scale;
			Quaternion rotation;
			std::string name;
			std::vector<Node*> children;
		};
		
		static Model* loadFBX(const char* path);

		Model();
		~Model();
		Node* buildTree(aiNode* aiNode);
		void fillMesh(Node* node);
		Mesh* createMesh(aiMesh* mesh);

		static std::map<std::string, Model*> __modelCache;
		static const int NullJoint = -1.0f;

		// temporal value for building scene tree
		const aiScene* _scene;
		Node* _root;
		std::map<std::string, Node*> _nodes;
	};
}

#endif