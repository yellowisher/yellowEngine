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
#include "yellowEngine/Component/SkinnedMeshRenderer.hpp"
#include "yellowEngine/Animation/AnimationClip.hpp"

namespace yellowEngine
{
	class Model
	{
		friend class AnimationClip;
	public:
		static Model* create(const char* path);

		GameObject* instantiate(const char* name, Material* material = nullptr);
		AnimationClip* getClip(int index)
		{
			auto it = _clips.begin();
			while (index-- > 0) it++;
			return it->second;
		}
		AnimationClip* getClip(const char* name)
		{
			auto it = _clips.find(name);
			if (it != _clips.end()) return it->second;
			return nullptr;
		}
		const std::map<std::string, AnimationClip*>& getClips() { return _clips; }
		void linkJoints(SkinnedMeshRenderer* renderer);

	private:
		struct Node
		{
			std::string name;
			bool hasBones;

			union
			{
				Mesh* mesh;
				aiMesh* aiMesh;
			};
			Material* material;

			Vector3 position;
			Vector3 scale;
			Quaternion rotation;
			Matrix offset;

			Node* parent;
			std::vector<Node*> children;

			// store instantiated transform
			Transform* transform;
			std::vector<Node*> jointNodes;
		};

		static Model* loadFBX(std::string path);

		static std::map<std::string, Model*> __modelCache;
		static constexpr float NullWeight = 0.0f;

		Model();
		~Model();
		Node* buildTree(aiNode* aiNode, const aiScene* scene);
		void fillMesh(Node* node, const aiScene* scene);
		std::pair<Mesh*, Material*> createMesh(aiMesh* mesh, const aiScene* scene, Node* currentNode);
		void copyMatrix(aiMatrix4x4& aiMatrix, Matrix& matrix);

		std::string _path;
		std::string _directory;

		bool _hasAnimation;
		Node* _root;
		std::map<std::string, Node*> _nodes;
		std::map<std::string, AnimationClip*> _clips;
	};
}

#endif