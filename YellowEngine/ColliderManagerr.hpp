#ifndef __H_COLLIDERMANAGER__
#define __H_COLLIDERMANAGER__

#include <vector>
#include <unordered_map>

#include "Collider.hpp"
#include "BroadPhase.hpp"

class ColliderManager
{
public:
	enum BroadPhaseType
	{
		BroadPhaseType_None,
		BroadPhaseType_SAP,
		BroadPhaseType_BVH
	};

	ColliderManager(BroadPhaseType type);
	~ColliderManager();

	void detect();
	void colliderCreated(Collider* collider);
	void colliderDestroyed(Collider* collider);
	void colliderUpdated(Collider* collider);
	void renderColliders();

private:
	enum PairType
	{
		PairType_New,
		PairType_Continuous,
		PairType_Delete
	};

	BroadPhase* _broadPhase;
	std::unordered_map<ColliderPair, PairType> _collidingPairs;
	std::vector<Collider*> _colliders;
};

#endif