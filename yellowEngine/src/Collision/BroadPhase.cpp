#include "yellowEngine/Collision/BroadPhase.hpp"


namespace yellowEngine
{
	BroadPhase::BroadPhase()
	{
	}


	BroadPhase::~BroadPhase()
	{
	}


	const std::unordered_set<ColliderPair>& BroadPhase::getPotentialPairs()
	{
		detect();
		return _pairs;
	}
}