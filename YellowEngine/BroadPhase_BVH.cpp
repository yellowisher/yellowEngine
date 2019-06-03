#include <stack>
#include <queue>

#include "Utils.hpp"
#include "BroadPhase_BVH.hpp"


// Node
void Node::constructor()
{
	state = 0;
	parent = rightChild = leftChild = NullObject;
}


void Node::destructor()
{
	state = NullObject;
}


// BroadPhase_BVH
BroadPhase_BVH::BroadPhase_BVH()
{
	_root = NullObject;
	expandFactor = 0.05f;
}


BroadPhase_BVH::~BroadPhase_BVH()
{
}


void BroadPhase_BVH::updateObject(Collider* target)
{
	ObjectId id = _nodeMap[target];
	updateNode(id, target->getBoundingBox());
}


void BroadPhase_BVH::addObjcet(Collider* target)
{
	ObjectId id = _nodePool.createObject();
	_nodeMap.insert({ target, id });

	Node& node = _nodePool.getObject(id);
	node.collider = target;

	node.aabb = target->getBoundingBox().expand(expandFactor);
	insertNode(id);
}


void BroadPhase_BVH::removeObject(Collider* target)
{
	ObjectId id = _nodeMap[target];
	deleteNode(id);
	_nodeMap.erase(target);
	_nodePool.returnObject(id);

	for (auto it = _pairs.begin(); it != _pairs.end();)
	{
		if (it->ca == target || it->cb == target)
		{
			it = _pairs.erase(it);
		}
		else
		{
			++it;
		}
	}
}


void BroadPhase_BVH::render(Renderer& renderer, ShaderProgram* shader, const Uniform* colorUniform)
{
	std::queue<ObjectId> visitQueue;
	visitQueue.push(_root);

	shader->setUniform(colorUniform, Vector3(1.0f, 1.0f, 1.0f));

	while (!visitQueue.empty())
	{
		ObjectId visit = visitQueue.front();
		visitQueue.pop();

		// render only branch nodes
		if (visit == NullObject)break;

		Node& visitNode = _nodePool[visit];
		if (visitNode.isLeaf())continue;

		renderer.setData(visitNode.aabb);
		renderer.render();

		visitQueue.push(visitNode.leftChild);
		visitQueue.push(visitNode.rightChild);
	}
}


void BroadPhase_BVH::detect()
{
	_pairs.clear();
	std::stack<ObjectId> visitStack;
	for (auto pair : _nodeMap)
	{
		ObjectId leafId = pair.second;
		Collider* collider = _nodePool[leafId].collider;
		AABB& aabb = _nodePool[leafId].aabb;

		visitStack.push(_root);
		while (!visitStack.empty())
		{
			ObjectId visit = visitStack.top();
			visitStack.pop();
			if (visit == leafId)continue;

			Node& visitNode = _nodePool[visit];

			if (aabb.isCollideWith(visitNode.aabb))
			{
				if (visitNode.isLeaf())
				{
					_pairs.insert(ColliderPair(collider, visitNode.collider));
				}
				else
				{
					visitStack.push(visitNode.leftChild);
					visitStack.push(visitNode.rightChild);
				}
			}
		}
	}
}


void BroadPhase_BVH::insertNode(ObjectId id)
{
	if (_root == NullObject)
	{
		_root = id;
		return;
	}

	int cursor = _root;
	while (!_nodePool[cursor].isLeaf())
	{
		Node& newNode = _nodePool[id];
		// find the best sibling for new inserted node
		// I assumed the best sibling as when combined, sibling node that produces least empty volume
		// calculating 'real' empty volume might be too expansive so just using approximate value

		// ok... I just figured out why box2d uses surface area as cost rather than volume
		// http://box2d.org/files/GDC2019/ErinCatto_DynamicBVH_Full.pdf

		Node& siblingNode = _nodePool[cursor];
		Node& leftChildNode = _nodePool[siblingNode.leftChild];
		Node& rightChildNode = _nodePool[siblingNode.rightChild];

		float newVolume = newNode.aabb.volume();
		float leftChildVolume = leftChildNode.aabb.volume();
		float rightChildVolume = rightChildNode.aabb.volume();

		// empty volume = combined volume - (each volume of children)
		float combined = siblingNode.aabb.volume();
		float realSum = leftChildVolume + rightChildVolume;
		float costSibling = combined - realSum;

		combined = AABB::combine(newNode.aabb, leftChildNode.aabb).volume();
		realSum = newVolume + leftChildVolume;
		float costLeft = combined - realSum;

		combined = AABB::combine(newNode.aabb, rightChildNode.aabb).volume();
		realSum = newVolume + rightChildVolume;
		float costRight = combined - realSum;

		if (costSibling < costLeft && costSibling < costRight)
		{
			break;
		}
		else if (costLeft < costRight)
		{
			cursor = siblingNode.leftChild;
		}
		else
		{
			cursor = siblingNode.rightChild;
		}
	}

	ObjectId newParent = _nodePool.createObject();
	Node& newParentNode = _nodePool[newParent];

	if (cursor == _root)
	{
		_root = newParent;
	}
	else
	{
		Node& grandParentNode = _nodePool[_nodePool[cursor].parent];

		if (cursor == grandParentNode.leftChild)
		{
			grandParentNode.leftChild = newParent;
		}
		else
		{
			grandParentNode.rightChild = newParent;
		}
		newParentNode.parent = _nodePool[cursor].parent;
	}
	newParentNode.leftChild = cursor;
	newParentNode.rightChild = id;

	_nodePool[cursor].parent = _nodePool[id].parent = newParent;

	ObjectId cur = newParent;
	while (cur != NullObject)
	{
		adjust(cur);

		cur = balance(cur);
		cur = _nodePool[cur].parent;
	}
}


void BroadPhase_BVH::deleteNode(ObjectId id)
{
	if (id == _root)
	{
		_root = NullObject;
		return;
	}
	Node& targetNode = _nodePool[id];
	Node& parentNode = _nodePool[targetNode.parent];

	ObjectId sibling;
	if (id == parentNode.leftChild)
	{
		sibling = parentNode.rightChild;
	}
	else
	{
		sibling = parentNode.leftChild;
	}

	Node& siblingNode = _nodePool[sibling];

	if (siblingNode.parent == _root)
	{
		_root = sibling;
	}
	else
	{
		Node& grandParentNode = _nodePool[parentNode.parent];
		if (grandParentNode.leftChild == siblingNode.parent)
		{
			grandParentNode.leftChild = sibling;
		}
		else
		{
			grandParentNode.rightChild = sibling;
		}
	}
	siblingNode.parent = parentNode.parent;
	adjustAscending(siblingNode.parent);

	// return only parent node; target node handled outside
	// because updateNode() function uses this function while updating
	_nodePool.returnObject(targetNode.parent);
}


void BroadPhase_BVH::updateNode(ObjectId id, AABB aabb)
{
	Node& node = _nodePool.getObject(id);
	// expanded aabb already contains new aabb
	if (node.aabb.contains(aabb)) return;

	node.aabb = aabb.expand(expandFactor);
	deleteNode(id);
	insertNode(id);
}


ObjectId BroadPhase_BVH::adjust(ObjectId target)
{
	Node& targetNode = _nodePool[target];
	Node& leftChildNode = _nodePool[targetNode.leftChild];
	Node& rightChildNode = _nodePool[targetNode.rightChild];

	targetNode.height = 1 + Utils::max(leftChildNode.height, rightChildNode.height);
	targetNode.aabb = AABB::combine(leftChildNode.aabb, rightChildNode.aabb);
	return targetNode.parent;
}


void BroadPhase_BVH::adjustAscending(ObjectId target)
{
	int cursor = target;
	while (cursor != NullObject)
	{
		cursor = adjust(cursor);
	}
}


ObjectId BroadPhase_BVH::balance(ObjectId target)
{
	Node& targetNode = _nodePool[target];
	if (targetNode.height < 2)return target;

	ObjectId leftChild = targetNode.leftChild;
	ObjectId rightChild = targetNode.rightChild;

	Node& leftChildNode = _nodePool[leftChild];
	Node& rightChildNode = _nodePool[rightChild];

	int balanceFactor = leftChildNode.height - rightChildNode.height;

	if (balanceFactor >= 2)
	{
		// right rotation
		ObjectId llChild = leftChildNode.leftChild;
		ObjectId lrChild = leftChildNode.rightChild;

		Node& llChildNode = _nodePool[llChild];
		Node& lrChildNode = _nodePool[lrChild];

		leftChildNode.parent = targetNode.parent;
		if (target == _root)
		{
			_root = leftChild;
		}
		else
		{
			Node& grandParentNode = _nodePool[targetNode.parent];
			if (target == grandParentNode.leftChild)
			{
				grandParentNode.leftChild = leftChild;
			}
			else
			{
				grandParentNode.rightChild = leftChild;
			}
		}

		targetNode.parent = leftChild;

		if (llChildNode.height > lrChildNode.height)
		{
			leftChildNode.rightChild = target;
			targetNode.leftChild = lrChild;
			lrChildNode.parent = target;
		}
		else
		{
			leftChildNode.leftChild = target;
			targetNode.leftChild = llChild;
			llChildNode.parent = target;
		}
		adjust(target);
		adjust(leftChild);
		return leftChild;
	}
	else if (balanceFactor <= -2)
	{
		// left rotation
		ObjectId rlChild = rightChildNode.leftChild;
		ObjectId rrChild = rightChildNode.rightChild;

		Node& rlChildNode = _nodePool[rlChild];
		Node& rrChildNode = _nodePool[rrChild];

		rightChildNode.parent = targetNode.parent;
		if (target == _root)
		{
			_root = rightChild;
		}
		else
		{
			Node& grandParentNode = _nodePool[targetNode.parent];
			if (target == grandParentNode.leftChild)
			{
				grandParentNode.leftChild = rightChild;
			}
			else
			{
				grandParentNode.rightChild = rightChild;
			}
		}
		targetNode.parent = rightChild;

		if (rlChildNode.height > rrChildNode.height)
		{
			rightChildNode.rightChild = target;
			targetNode.rightChild = rrChild;
			rrChildNode.parent = target;
		}
		else
		{
			rightChildNode.leftChild = target;
			targetNode.rightChild = rlChild;
			rlChildNode.parent = target;
		}
		adjust(target);
		adjust(rightChild);
		return rightChild;
	}
	return target;
}