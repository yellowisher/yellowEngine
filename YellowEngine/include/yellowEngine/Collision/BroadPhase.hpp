#ifndef __H_BROADPHASE__
#define __H_BROADPHASE__

#include <unordered_set>

#include "yellowEngine/Component/Collider.hpp"
#include "yellowEngine/Rendering/Renderer.hpp"

struct ColliderPair
{
	ColliderPair(Collider* a, Collider* b)
	{
		if (a > b)
		{
			ca = b;
			cb = a;
		}
		else
		{
			ca = a;
			cb = b;
		}
	}

	bool operator==(const ColliderPair& other) const
	{
		return (ca == other.ca && cb == other.cb) || (ca == other.cb && cb == other.ca);
	}

	Collider* ca;
	Collider* cb;
};

namespace std
{
	template <>
	class hash<ColliderPair>
	{
	public:
		size_t operator()(const ColliderPair& pair) const
		{
			return (size_t)pair.ca * 2654435761 + (size_t)pair.cb * 2654435761;
		}
	};
};

class BroadPhase
{
public:
	BroadPhase();
	virtual ~BroadPhase();

	const std::unordered_set<ColliderPair>& getPotentialPairs();
	virtual void updateObject(Collider* target) = 0;
	virtual void addObjcet(Collider* target) = 0;
	virtual void removeObject(Collider* target) = 0;
	virtual void render(Renderer& renderer, ShaderProgram* shader, const Uniform* colorUniform) = 0;

protected:
	virtual void detect() = 0;

	std::unordered_set<ColliderPair> _pairs;
};

#endif