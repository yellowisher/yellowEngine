#ifndef __H_MODEL__
#define __H_MODEL__

#include <map>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "yellowEngine/Rendering/Material.hpp"
#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Math/Quaternion.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Component/GameObject.hpp"

namespace yellowEngine
{
	class Model
	{
	public:
		static Model* create(const char* path, bool absolute = false);
		
		GameObject* instantiate(const char* name);
		
	private:
		struct Node	
		{
			union
			{
				Mesh* mesh;
				aiMesh* aiMesh;
			};
			Material material;

			Vector3 position;
			Vector3 scale;
			Quaternion rotation;
			std::string name;
			std::vector<Node*> children;
			
			// store instantiated transform
			Transform* transform;
			std::vector<Node*> jointNodes;
		};
		
		static Model* loadFBX(std::string path);

		static std::map<std::string, Model*> __modelCache;
		static constexpr float NullJoint = -1.0f;

		Model();
		~Model();
		Node* buildTree(aiNode* aiNode);
		void fillMesh(Node* node);
		std::pair<Mesh*, Material> createMesh(aiMesh* mesh, Node* currentNode);

		// temporal value for building scene tree
		const aiScene* _scene;
		std::string _directory;

		Node* _root;
		std::map<std::string, Node*> _nodes;
	};
}

#endif