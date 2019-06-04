#include "yellowEngine/System/GameObject.hpp"
#include "yellowEngine/Collision/BroadPhase_SAP.hpp"

#include <iostream>
using namespace std;


BroadPhase_SAP::BroadPhase_SAP()
{
}


BroadPhase_SAP::~BroadPhase_SAP()
{
}


void BroadPhase_SAP::updateObject(Collider* target)
{
	AABB aabb = target->getBoundingBox();

	Proxy* proxy = &_proxies[target];
	proxy->points[X][Min]->value = aabb.min().x;
	proxy->points[Y][Min]->value = aabb.min().y;
	proxy->points[Z][Min]->value = aabb.min().z;
	proxy->points[X][Max]->value = aabb.max().x;
	proxy->points[Y][Max]->value = aabb.max().y;
	proxy->points[Z][Max]->value = aabb.max().z;
}


// another version of adding object
// find right place and update proxy link
// but cannot figure out how to detect newly created pair; any idea?

void BroadPhase_SAP::addObjcet(Collider* target)
{
	_proxies.insert({ target, Proxy(target) });
	Proxy* proxy = &_proxies[target];

	AABB aabb = target->getBoundingBox();
	bool grow = false;

	for (int axis = 0; axis < Num_Axis; axis++)
	{
		auto& endPoints = _endPoints[axis];
		if (endPoints.size() + 2 > endPoints.capacity())
		{
			grow = true;
		}
		int i = endPoints.size() - 1;
		endPoints.resize(endPoints.size() + 2);

		for (int m = Max; m >= Min; m--)
		{
			for (; i >= 0; i--)
			{
				if (endPoints[i].value > aabb[m].v[axis])
				{
					endPoints[i + m + 1] = endPoints[i];
				}
				else
				{
					break;
				}
			}
			endPoints[i + m + 1] = EndPoint(proxy, aabb[m].v[axis], (MinMax)m);
		}

		// if vector did not grow, re-link only some part
		if (!grow)
		{
			reLinkAxisFrom(axis, i + 1);
		}
	}
	// if vector did grow, re-link all of points
	if (grow)
	{
		reLinkAllAxis();
	}

	// add new potential pairs;
	int bi = proxy->points[X][Min] - &_endPoints[X][0];
	int ei = proxy->points[X][Max] - &_endPoints[X][0];

	vector<bool> opens = vector<bool>(ei, false);
	for (int i = 0; i < bi; i++)
	{
		if (_endPoints[X][i].type == Min)opens[i] = true;
		else opens[i] = false;
	}
	for (int i = bi + 1; i < ei; i++)
	{
		if (_endPoints[X][i].type == Min)opens[i] = true;
	}

	for (int i = 0; i < ei; i++)
	{
		if (opens[i])
		{
			if (isOverlap(proxy, _endPoints[X][i].proxy))
			{
				addPair(proxy, _endPoints[X][i].proxy);
			}
		}
	}
}

//void BroadPhase_SAP::addObjcet(Collider* target)
//{
//	_proxies.insert({ target, Proxy(target) });
//	Proxy* proxy = &_proxies[target];
//
//	const AABB aabb = target->getAABB();
//	bool grow = false;
//
//	for (int axis = 0; axis < Num_Axis; axis++)
//	{
//		auto& endPoints = _endPoints[axis];
//		if (endPoints.size() + 2 > endPoints.capacity())
//		{
//			grow = true;
//		}
//		int i = endPoints.size() - 1;
//		endPoints.push_back(EndPoint(proxy, aabb.min.v[axis], Min));
//		endPoints.push_back(EndPoint(proxy, aabb.max.v[axis], Max));
//
//		// if vector did not grow, re-link only two point
//		if (!grow)
//		{
//			reLinkPoint(axis, endPoints[endPoints.size() - 1]);
//			reLinkPoint(axis, endPoints[endPoints.size() - 2]);
//		}
//	}
//	// if vector did grow, re-link all of points
//	if (grow)
//	{
//		reLinkAllAxis();
//	}
//}


void BroadPhase_SAP::removeObject(Collider* target)
{
	auto it = _proxies.find(target);
	if (it == _proxies.end())return;

	Proxy* proxy = &it->second;

	// remove potential pairs first
	for (auto it = _pairs.begin(); it != _pairs.end();)
	{
		if (it->ca == proxy->owner || it->cb == proxy->owner)
		{
			it = _pairs.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (int axis = 0; axis < Num_Axis; axis++)
	{
		auto& endPoints = _endPoints[axis];

		int begin = proxy->points[axis][Min] - &endPoints[0];
		int end = proxy->points[axis][Max] - &endPoints[0];

		for (int i = begin + 1; i < end; i++)
		{
			endPoints[i - 1] = endPoints[i];
		}

		for (size_t i = end + 1; i < endPoints.size(); i++)
		{
			endPoints[i - 2] = endPoints[i];
		}

		endPoints.pop_back();
		endPoints.pop_back();

		reLinkAxisFrom(axis, begin);
	}

	_proxies.erase(it);
}


bool BroadPhase_SAP::isOverlap(Proxy* pa, Proxy* pb)
{
	if (pa->points[X][Min]->value > pb->points[X][Max]->value || pb->points[X][Min]->value > pa->points[X][Max]->value)return false;
	if (pa->points[Y][Min]->value > pb->points[Y][Max]->value || pb->points[Y][Min]->value > pa->points[Y][Max]->value)return false;
	if (pa->points[Z][Min]->value > pb->points[Z][Max]->value || pb->points[Z][Min]->value > pa->points[Z][Max]->value)return false;
	return true;
}


void BroadPhase_SAP::addPair(Proxy* pa, Proxy* pb)
{
	cout << "New Potential " << pa->owner->gameObject->getName() << " and " <<
		pb->owner->gameObject->getName() << endl;
	_pairs.insert(ColliderPair(pa->owner, pb->owner));
}


void BroadPhase_SAP::removePair(Proxy* pa, Proxy* pb, bool check)
{
	if (check)
	{
		auto it = _pairs.find(ColliderPair(pa->owner, pb->owner));
		if (it != _pairs.end())
		{
			cout << "Remove Potential " << pa->owner->gameObject->getName() << " and " <<
				pb->owner->gameObject->getName() << endl;

			_pairs.erase(it);
		}
	}
	else
	{
		_pairs.erase(ColliderPair(pa->owner, pb->owner));
	}
}


void BroadPhase_SAP::detect()
{
	for (int axis = 0; axis < Num_Axis; axis++)
	{
		auto& endPoints = _endPoints[axis];
		for (size_t t = 1; t < endPoints.size(); t++)
		{
			EndPoint target = endPoints[t];
			int i = t - 1;
			for (; i >= 0; i--)
			{
				if (target.value < endPoints[i].value)
				{
					if (target.type == Min && endPoints[i].type == Max)
					{
						cout << "One Axis isOverlapped" << endl;
						if (isOverlap(target.proxy, endPoints[i].proxy))
						{
							// new potential collision detected
							addPair(target.proxy, endPoints[i].proxy);
						}
					}
					else if (target.type == Max && endPoints[i].type == Min)
					{
						// collision ended
						removePair(target.proxy, endPoints[i].proxy);
					}

					endPoints[i + 1] = endPoints[i];
					reLinkPoint(axis, endPoints[i + 1]);
				}
				else
				{
					break;
				}
			}
			endPoints[i + 1] = target;
			reLinkPoint(axis, endPoints[i + 1]);
		}
	}
}


void BroadPhase_SAP::reLinkPoint(int axis, EndPoint& endPoint)
{
	endPoint.proxy->points[axis][endPoint.type] = &endPoint;
}


void BroadPhase_SAP::reLinkAxisFrom(int axis, int start)
{
	for (size_t i = start; i < _endPoints[axis].size(); i++)
	{
		reLinkPoint(axis, _endPoints[axis][i]);
	}
}


void BroadPhase_SAP::reLinkAllAxis()
{
	for (int axis = 0; axis < Num_Axis; axis++)
	{
		for (auto& endPoint : _endPoints[axis])
		{
			reLinkPoint(axis, endPoint);
		}
	}
}