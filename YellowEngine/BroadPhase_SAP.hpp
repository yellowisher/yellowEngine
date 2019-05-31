#ifndef __H_BROADPHASE_SAP__
#define __H_BROADPHASE_SAP__

#include <list>
#include <map>
#include <vector>

#include "BroadPhase.hpp"
#include "Collider.hpp"

class BroadPhase_SAP : public BroadPhase
{
public:
	BroadPhase_SAP();
	~BroadPhase_SAP();

	void updateObject(Collider* target) override;
	void addObjcet(Collider* target) override;
	void removeObject(Collider* target) override;

protected:
	void detect() override;

private:
	enum Axis
	{
		X,
		Y,
		Z,
		Num_Axis
	};

	enum MinMax
	{
		Min,
		Max,
		Num_MinMax
	};

	struct Proxy;
	struct EndPoint
	{
		EndPoint() {}
		EndPoint(Proxy* proxy, float value, MinMax type) :proxy(proxy), value(value), type(type) {}

		Proxy* proxy;
		float value;
		MinMax type;
	};

	struct Proxy
	{
		Proxy() {}
		Proxy(Collider* owner) :owner(owner) {}

		Collider* owner;
		EndPoint* points[Num_Axis][Num_MinMax];
	};

	// batch?
	void addPair(Proxy* pa, Proxy* pb);
	bool overlap(Proxy* pa, Proxy* pb);
	void removePair(Proxy* pa, Proxy* pb, bool check = true);
	
	void reLinkPoint(int axis, EndPoint& endPoint);
	void reLinkAxisFrom(int axis, int start);
	void reLinkAllAxis();

	std::vector<EndPoint> _endPoints[Num_Axis];
	std::map<Collider*, Proxy> _proxies;
};

#endif