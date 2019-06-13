#ifndef __H_BROADPHASE_BVH__
#define __H_BROADPHASE_BVH__

#include <map>

#include "yellowEngine/Collision/BroadPhase.hpp"
#include "yellowEngine/Utility/ObjectPool.hpp"
#include "yellowEngine/Math/AABB.hpp"

namespace yellowEngine
{
	class Node : Poolable
	{
		friend class ObjectPool<Node>;

	public:
		void constructor() override;
		void destructor() override;

		inline bool isLeaf() { return rightChild == NullObject; }

		int height;
		union
		{
			int parent;
			int next;
		};
		union
		{
			// only leaf node contains collider data; user leftChild as collider pointer
			struct
			{
				int leftChild;
				int PADDING_FOR_64BIT;
				int rightChild;
			};
			Collider* collider;
		};
		AABB aabb;

	private:
		Node() {}
		~Node() {}
	};


	class BroadPhase_BVH : public BroadPhase
	{
	public:
		BroadPhase_BVH();
		~BroadPhase_BVH();

		void updateObject(Collider* target) override;
		void addObjcet(Collider* target) override;
		void removeObject(Collider* target) override;
		void render(Renderer& renderer, Shader* shader, const Uniform* colorUniform) override;

		float expandFactor;

	protected:
		void detect() override;

	private:
		void insertNode(ObjectId id);
		void deleteNode(ObjectId id);
		void updateNode(ObjectId id, AABB aabb);
		ObjectId adjust(ObjectId target);
		ObjectId balance(ObjectId target);

		ObjectPool<Node> _nodePool;
		ObjectId _root;

		std::map<Collider*, ObjectId> _nodeMap;
	};
}

#endif