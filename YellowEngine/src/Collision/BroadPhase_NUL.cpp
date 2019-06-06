#include "yellowEngine/Collision/BroadPhase_NUL.hpp"


namespace yellowEngine
{
	void BroadPhase_NUL::detect()
	{
		ColliderManager* manager = ColliderManager::getInstance();
		auto _colliders = manager->_colliders;
		auto _collidingPairs = manager->_collidingPairs;

		for (size_t i = 0; i < _colliders.size() - 1; i++)
		{
			for (size_t j = i + 1; j < _colliders.size(); j++)
			{
				if (_colliders[i]->isCollideWith(_colliders[j]))
				{
					auto pair = ColliderPair(_colliders[i], _colliders[j]);
					auto it = _collidingPairs.find(pair);
					if (it != _collidingPairs.end())
					{
						it->second = ColliderManager::PairType_Continuous;
						manager->collisionStay(pair);
					}
					else
					{
						_collidingPairs.insert({ pair, ColliderManager::PairType_New });
						manager->collisionEnter(pair);
					}
				}
			}
		}
	}
}