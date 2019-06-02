#ifndef __H_BROADPHASE_BVH__
#define __H_BROADPHASE_BVH__

#include <map>

#include "BroadPhase.hpp"
#include "ObjectPool.hpp"
#include "AABB.hpp"


class Node : Poolable
{
	friend class ObjectPool<Node>;

public:
	void constructor() override;
	void destructor() override;

	inline bool isLeaf() { return rightChild == NullObject; }

	union
	{
		int height;
		int state;
	};
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

	float expandFactor;

protected:
	void detect() override;

private:
	void insertNode(ObjectId id);
	void deleteNode(ObjectId id);
	void updateNode(ObjectId id, AABB aabb);
	void adjustAscending(ObjectId target);
	ObjectId adjust(ObjectId target);
	ObjectId balance(ObjectId target);

	ObjectPool<Node> _nodePool;
	ObjectId _root;

	std::map<Collider*, ObjectId> _nodeMap;
};

#endif