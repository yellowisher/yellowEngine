#include "BroadPhase_SAP.hpp"
#include "BroadPhase_BVH.hpp"
#include "ColliderManagerr.hpp"

#include <iostream>
using namespace std;


ColliderManager* ColliderManager::_instance = nullptr;


ColliderManager* ColliderManager::create(BroadPhaseType type)
{
	if (_instance != nullptr) return nullptr;
	_instance = new ColliderManager(type);
	return _instance;
}


ColliderManager* ColliderManager::getInstance()
{
	return _instance;
}


ColliderManager::ColliderManager(BroadPhaseType type)
{
	_broadPhase = nullptr;
	if (type == BroadPhaseType_SAP)_broadPhase = new BroadPhase_SAP();
	else if (type == BroadPhaseType_BVH)_broadPhase = new BroadPhase_BVH();
}


ColliderManager::~ColliderManager()
{
}


void ColliderManager::colliderCreated(Collider* collider)
{
	if (_broadPhase)
	{
		_broadPhase->addObjcet(collider);
	}

	_colliders.push_back(collider);
}


void ColliderManager::colliderDestroyed(Collider* collider)
{
	if (_broadPhase)
	{
		_broadPhase->removeObject(collider);
	}

	for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
	{
		if (*it == collider)
		{
			_colliders.erase(it);
			break;
		}
	}

	for (auto it = _collidingPairs.begin(); it != _collidingPairs.end();)
	{
		if (it->first.ca == collider || it->first.cb == collider)
		{
			cout << "Exit: " << it->first.ca->gameObject->getName() << " and " << it->first.cb->gameObject->getName() << endl;
			it = _collidingPairs.erase(it);
		}
		else
		{
			++it;
		}
	}
}


void ColliderManager::colliderUpdated(Collider* collider)
{
	if (_broadPhase)
	{
		_broadPhase->updateObject(collider);
	}
}


void ColliderManager::renderColliders()
{
	for (auto collider : _colliders)
	{
		collider->renderCollider();
	}
}



void ColliderManager::detect()
{
	if (_broadPhase == nullptr)
	{
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
						it->second = PairType_Continuous;
					}
					else
					{
						cout << "Enter: " << pair.ca->gameObject->getName() << " and " << pair.cb->gameObject->getName() << endl;
						_collidingPairs.insert({ pair, PairType_New });
					}
				}
			}
		}
	}
	else
	{
		auto& potentialPairs = _broadPhase->getPotentialPairs();
		for (auto pair : potentialPairs)
		{
			if (pair.ca->isCollideWith(pair.cb))
			{
				auto it = _collidingPairs.find(pair);
				if (it != _collidingPairs.end())
				{
					// is collide but isn't new
					// OnColliderStay

					it->second = PairType_Continuous;
				}
				else
				{
					// is collide and is new; add to collding list
					// OnColliderEnter

					cout << "Enter: " << pair.ca->gameObject->getName() << " and " << pair.cb->gameObject->getName() << endl;
					_collidingPairs.insert({ pair, PairType_New });
				}
			}
		}
	}

	for (auto it = _collidingPairs.begin(); it != _collidingPairs.end();)
	{
		if (it->second == PairType_Delete)
		{
			// OnColliderExit
			cout << "Exit: " << it->first.ca->gameObject->getName() << " and " << it->first.cb->gameObject->getName() << endl;
			it = _collidingPairs.erase(it);
		}
		else
		{
			it->second = PairType_Delete;
			++it;
		}
	}
}