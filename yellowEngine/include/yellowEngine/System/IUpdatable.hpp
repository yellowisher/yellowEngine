#ifndef __H_IUPDATABLE__
#define __H_IUPDATABLE__

#include "yellowEngine/System/Game.hpp"

namespace yellowEngine
{
	class IUpdatable
	{
	public:
		IUpdatable()
		{
			Game::addUpdatable(this);
		}
		virtual ~IUpdatable()
		{
			Game::removeUpdatable(this);
		}
		virtual void update() = 0;
	};
}

#endif