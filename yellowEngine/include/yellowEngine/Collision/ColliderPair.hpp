#ifndef __H_COLLIDERPAIR__
#define __H_COLLIDERPAIR__

#include "yellowEngine/Component/Collider.hpp"

struct ColliderPair
{
	ColliderPair(yellowEngine::Collider* a, yellowEngine::Collider* b)
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

	yellowEngine::Collider* ca;
	yellowEngine::Collider* cb;
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

#endif