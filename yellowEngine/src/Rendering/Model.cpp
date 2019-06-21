#include <queue>
#include <stack>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Utility/Definitions.hpp"
#include "yellowEngine/System/Game.hpp"
#include "yellowEngine/Component/MeshRenderer.hpp"
#include "yellowEngine/Component/SkinnedMeshRenderer.hpp"
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
			node->transform = gameObject->transform;
			if (node->mesh)
			{
				if (_hasAnimation)
				{
					gameObject->addComponent<SkinnedMeshRenderer>();
				}
				else
				{
					gameObject->addComponent<MeshRenderer>()->set(node->mesh, node->material);
				}
			}

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

		if (_hasAnimation)
		{
			visitQueue.push({ _root, nullptr });
			while (!visitQueue.empty())
			{
				auto visit = visitQueue.front();
				Node* node = visit.first;
				visitQueue.pop();

				auto renderer = node->transform->gameObject->getComponent<SkinnedMeshRenderer>();
				if (renderer != nullptr)
				{
					std::vector<std::pair<Transform*, Matrix>> joints;
					for (auto node : node->jointNodes)
					{
						joints.push_back({ node->transform,node->offset });
					}
					renderer->set(node->mesh, node->material, joints, rootObject->transform);
				}

				for (auto child : node->children)
				{
					visitQueue.push({ child, nullptr });
				}
			}
		}

		return rootObject;
	}


	Model* Model::create(const char* path, bool absolute)
	{
		std::string fullpath = path;
		if (!absolute) fullpath = Game::getAssetPath(path).c_str();

		auto it = __modelCache.find(fullpath);
		if (it != __modelCache.end())
		{
			return it->second;
		}

		Model* model = loadFBX(fullpath.c_str());
		__modelCache.insert({ fullpath, model });
		return model;
	}


	Model* Model::loadFBX(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Utils::print(importer.GetErrorString());
			return nullptr;
		}

		Model* model = new Model();

		// feed temporal information for building scene hierachy
		model->_hasAnimation = scene->HasAnimations();
		model->_directory = path.substr(0, path.find_last_of('/') + 1);

		model->_root = model->buildTree(scene->mRootNode, scene);

		// after build scene hierarchy, fill all the meshes
		model->fillMesh(model->_root, scene);

		// create animation clips
		if (scene->HasAnimations())
		{
			for (unsigned int a = 0; a < scene->mNumAnimations; a++)
			{
				aiAnimation* animation = scene->mAnimations[a];
				
				AnimationClip* clip = new AnimationClip();
				// TODO: where to find looping? and set play speed
				clip->_frameCount = (int)animation->mDuration;
				clip->_isLooping = true;
				for (unsigned int c = 0; c < animation->mNumChannels; c++)
				{
					aiNodeAnim* channel = animation->mChannels[c];
					auto targetNode = model->_nodes[channel->mNodeName.C_Str()];

					std::string targetPath = targetNode->name + '/';
					auto cursor = targetNode->parent;
					while (cursor != model->_root)
					{
						targetPath = cursor->name + '/' + targetPath;
						cursor = cursor->parent;
					}

					if (channel->mNumPositionKeys > 0)
					{
						AnimationClip::Key key = { targetPath, AnimationClip::Property_Position };
						clip->_channels.insert({ key, {} });
						std::vector<AnimationClip::KeyFrame>& frames = clip->_channels[key];
						frames.reserve(channel->mNumPositionKeys);

						for (unsigned int f = 0; f < channel->mNumPositionKeys; f++)
						{
							auto frame = channel->mPositionKeys[f];
							Vector3 position(frame.mValue.x, frame.mValue.y, frame.mValue.z);
							frames.push_back(AnimationClip::KeyFrame((int)frame.mTime, position));
						}
					}

					if (channel->mNumRotationKeys > 0)
					{
						AnimationClip::Key key = { targetPath, AnimationClip::Property_Rotation };
						clip->_channels.insert({ key, {} });
						std::vector<AnimationClip::KeyFrame>& frames = clip->_channels[key];
						frames.reserve(channel->mNumRotationKeys);

						for (unsigned int f = 0; f < channel->mNumRotationKeys; f++)
						{
							auto frame = channel->mRotationKeys[f];
							Quaternion rotation(frame.mValue.x, frame.mValue.y, frame.mValue.z, frame.mValue.w);
							frames.push_back(AnimationClip::KeyFrame((int)frame.mTime, rotation));
						}
					}

					if (channel->mScalingKeys > 0)
					{
						AnimationClip::Key key = { targetPath, AnimationClip::Property_Scale };
						clip->_channels.insert({ key, {} });
						std::vector<AnimationClip::KeyFrame>& frames = clip->_channels[key];
						frames.reserve(channel->mNumScalingKeys);

						for (unsigned int f = 0; f < channel->mNumScalingKeys; f++)
						{
							auto frame = channel->mScalingKeys[f];
							Vector3 scale(frame.mValue.x, frame.mValue.y, frame.mValue.z);
							frames.push_back(AnimationClip::KeyFrame((int)frame.mTime, scale));
						}
					}
				}
				model->_clips.insert({ animation->mName.C_Str() ,clip });
			}
		}

		return model;
	}


	void Model::fillMesh(Node* node, const aiScene* scene)
	{
		if (node->aiMesh != nullptr)
		{
			auto pair = createMesh(node->aiMesh, scene, node);

			node->mesh = pair.first;
			node->material = pair.second;
		}

		for (auto child : node->children)
		{
			fillMesh(child, scene);
		}
	}


	Model::Node* Model::buildTree(aiNode* aiNode, const aiScene* scene)
	{
		Node* node = new Node();
		node->parent = nullptr;

		// TODO: figure it out in which situation a node contains multiple meshes
		// current, handle atmost one mesh
		if (aiNode->mNumMeshes > 0)
		{
			node->aiMesh = scene->mMeshes[aiNode->mMeshes[0]];
		}
		else
		{
			node->aiMesh = nullptr;
		}

		Matrix matrix;
		copyMatrix(aiNode->mTransformation, matrix);

		node->position = matrix.extractTranslation();
		node->rotation = matrix.extractRotation();
		node->scale = matrix.extractScale();

		node->name = aiNode->mName.C_Str();
		_nodes.insert({ node->name, node });

		for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
		{
			Node* child = buildTree(aiNode->mChildren[i], scene);
			child->parent = node;
			node->children.push_back(child);
		}

		return node;
	}


	std::pair<Mesh*, Material> Model::createMesh(aiMesh* aiMesh, const aiScene* scene, Node* currentNode)
	{	
		// Model class already caching model itself
		// there is no need to cache in Mesh class
		// so create directly rather than call Mesh::create
		Mesh* mesh = nullptr;
		std::vector<AttributeUsage> attributes;

		//if (aiMesh->HasPositions())
		//{
		//	attributes.push_back(Attr_Position);
		//}
		//if (aiMesh->HasNormals())
		//{
		//	attributes.push_back(Attr_Normal);
		//}
		//if (aiMesh->HasTextureCoords(0))
		//{
		//	attributes.push_back(Attr_TexCoord0);
		//}
		//if (aiMesh->HasBones())
		//{
		//	attributes.push_back(Attr_Joints);
		//	attributes.push_back(Attr_Weights);
		//}

		//VertexLayout layout(attributes);
		//int stride = layout.getVertexSize() / sizeof(float);

		//int jointOffset = 0;
		//int weightOffset = 0;

		//float* vertices = new float[stride * aiMesh->mNumVertices];
		//for (auto attrPair : layout.getAttributes())
		//{
		//	const auto& attribute = attrPair.second;
		//	int cursor = attribute.offset / sizeof(float);
		//	switch (attribute.usage)
		//	{
		//		case Attr_Position:
		//		{
		//			for (int i = 0; i < aiMesh->mNumVertices; i++, cursor += stride)
		//			{
		//				vertices[cursor] = aiMesh->mVertices[i].x;
		//				vertices[cursor + 1] = aiMesh->mVertices[i].y;
		//				vertices[cursor + 2] = aiMesh->mVertices[i].z;
		//			}
		//		}
		//		break;

		//		case Attr_Normal:
		//		{
		//			for (int i = 0; i < aiMesh->mNumVertices; i++, cursor += stride)
		//			{
		//				vertices[cursor] = aiMesh->mNormals[i].x;
		//				vertices[cursor + 1] = aiMesh->mNormals[i].y;
		//				vertices[cursor + 2] = aiMesh->mNormals[i].z;
		//			}
		//		}
		//		break;

		//		case Attr_TexCoord0:
		//		{
		//			for (int i = 0; i < aiMesh->mNumVertices; i++, cursor += stride)
		//			{
		//				vertices[cursor] = aiMesh->mTextureCoords[0][i].x;
		//				vertices[cursor + 1] = aiMesh->mTextureCoords[0][i].y;
		//			}
		//		}
		//		break;

		//		case Attr_Joints:
		//		case Attr_Weights:
		//		{

		//			for (int i = 0; i < aiMesh->mNumVertices; i++, cursor += stride)
		//			{
		//				for (int j = 0; j < MaxJointCount; j++)
		//				{
		//					vertices[cursor + j] = NullWeight;
		//				}
		//			}
		//		}
		//		break;
		//	}
		//}

		//int* indices = new int[3 * aiMesh->mNumFaces];
		//for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
		//{
		//	indices[i * 3    ] = aiMesh->mFaces[i].mIndices[0];
		//	indices[i * 3 + 1] = aiMesh->mFaces[i].mIndices[1];
		//	indices[i * 3 + 2] = aiMesh->mFaces[i].mIndices[2];
		//}

		//currentNode->jointNodes.resize(aiMesh->mNumBones);
		//for (unsigned int b = 0; b < aiMesh->mNumBones; b++)
		//{
		//	Node* jointNode = _nodes[aiMesh->mBones[b]->mName.C_Str()];
		//	currentNode->jointNodes[b] = jointNode;
		//	for (unsigned int w = 0; w < aiMesh->mBones[b]->mNumWeights; w++)
		//	{
		//		int vi = aiMesh->mBones[b]->mWeights[w].mVertexId;
		//		int vi_j = stride * vi + jointOffset;
		//		int vi_w = stride * vi + weightOffset;

		//		int wi = 0;
		//		while (wi < MaxJointCount && vertices[vi_w + wi] != NullWeight) wi++;
		//		if (wi >= MaxJointCount) continue;

		//		vertices[vi_j + wi] = (float)b;
		//		vertices[vi_w + wi] = aiMesh->mBones[b]->mWeights[w].mWeight;
		//	}

		//	copyMatrix(aiMesh->mBones[b]->mOffsetMatrix, jointNode->offset);
		//}





		if (aiMesh->HasBones())
		{
			std::vector<Mesh::SkinnedVertex> vertices(aiMesh->mNumVertices);
			std::vector<unsigned int> indices(aiMesh->mNumFaces * 3);

			for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
			{
				vertices[i].position.x = aiMesh->mVertices[i].x;
				vertices[i].position.y = aiMesh->mVertices[i].y;
				vertices[i].position.z = aiMesh->mVertices[i].z;

				if (aiMesh->HasNormals())
				{
					vertices[i].normal.x = aiMesh->mNormals[i].x;
					vertices[i].normal.y = aiMesh->mNormals[i].y;
					vertices[i].normal.z = aiMesh->mNormals[i].z;
				}

				if (aiMesh->HasTextureCoords(0))
				{
					vertices[i].uv.x = aiMesh->mTextureCoords[0][i].x;
					vertices[i].uv.y = aiMesh->mTextureCoords[0][i].y;
				}

				for (int j = 0; j < MaxJointCount; j++)
				{
					vertices[i].joints[j] = 0;
					vertices[i].weights[j] = NullWeight;
				}
			}

			for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
			{
				indices[i * 3]	   = aiMesh->mFaces[i].mIndices[0];
				indices[i * 3 + 1] = aiMesh->mFaces[i].mIndices[1];
				indices[i * 3 + 2] = aiMesh->mFaces[i].mIndices[2];
			}

			currentNode->jointNodes.resize(aiMesh->mNumBones);
			for (unsigned int b = 0; b < aiMesh->mNumBones; b++)
			{
				Node* jointNode = _nodes[aiMesh->mBones[b]->mName.C_Str()];
				currentNode->jointNodes[b] = jointNode;
				for (unsigned int w = 0; w < aiMesh->mBones[b]->mNumWeights; w++)
				{
					int vi = aiMesh->mBones[b]->mWeights[w].mVertexId;

					int wi = 0;
					while (wi < MaxJointCount && vertices[vi].weights[wi] != NullWeight) wi++;
					if (wi >= MaxJointCount) continue;

					vertices[vi].joints[wi] = (float)b;
					vertices[vi].weights[wi] = aiMesh->mBones[b]->mWeights[w].mWeight;
				}

				copyMatrix(aiMesh->mBones[b]->mOffsetMatrix, jointNode->offset);
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

				vertices[i].uv.x = aiMesh->mTextureCoords[0][i].x;
				vertices[i].uv.y = aiMesh->mTextureCoords[0][i].y;
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

		Material mat;
		mat.setTechnique(Technique::getTechnique(TechniqueType_Deferred), "Shader/texture.vert", "Shader/texture.frag");

		aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			path = _directory + path.C_Str();

			mat.addTexture(Texture::create(path.C_Str(), true), "u_Material.diffuse");
		}

		if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			aiString path;
			material->GetTexture(aiTextureType_SPECULAR, 0, &path);
			path = _directory + path.C_Str();

			mat.addTexture(Texture::create(path.C_Str(), true), "u_Material.specular");
		}
		return { mesh, mat };
	}


	void Model::copyMatrix(aiMatrix4x4& aiMatrix, Matrix& matrix)
	{
		for (int i = 0; i < 16; i++)
		{
			matrix[i] = aiMatrix[i % 4][i / 4];
		}
	}
}