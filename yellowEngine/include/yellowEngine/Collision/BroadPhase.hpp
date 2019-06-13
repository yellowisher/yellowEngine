#ifndef __H_BROADPHASE__
#define __H_BROADPHASE__

#include <unordered_set>

#include "yellowEngine/Collision/ColliderPair.hpp"
#include "yellowEngine/Component/Collider.hpp"
#include "yellowEngine/Rendering/Renderer.hpp"

namespace yellowEngine
{
	class BroadPhase
	{
	public:
		BroadPhase();
		virtual ~BroadPhase();

		const std::unordered_set<ColliderPair>& getPotentialPairs();
		virtual void updateObject(Collider* target) = 0;
		virtual void addObjcet(Collider* target) = 0;
		virtual void removeObject(Collider* target) = 0;
		virtual void render(Renderer& renderer, Shader* shader, const Uniform* colorUniform) = 0;

	protected:
		virtual void detect() = 0;

		std::unordered_set<ColliderPair> _pairs;
	};
}

#endif