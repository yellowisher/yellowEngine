#include "yellowEngine/System/IUpdatable.hpp"


namespace yellowEngine
{
	void(*IUpdatable::reportCreate) (IUpdatable* updatable);
	void(*IUpdatable::reportDestroy)(IUpdatable* updatable);
}