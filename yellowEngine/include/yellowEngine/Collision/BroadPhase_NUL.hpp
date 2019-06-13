#ifndef __H_BROADPHASENUL__
#define __H_BROADPHASENUL__

#include "yellowEngine/Collision/ColliderManager.hpp"
#include "yellowEngine/Collision/BroadPhase.hpp"

namespace yellowEngine
{
	class BroadPhase_NUL : public BroadPhase
	{
	public:
		BroadPhase_NUL() {}
		~BroadPhase_NUL() {}

		void updateObject(Collider* target) override {}
		void addObjcet(Collider* target) override {}
		void removeObject(Collider* target) override {}
		void render(Renderer& renderer, Shader* shader, const Uniform* colorUniform) override {}

	protected:
		void detect() override;
	};
}

#endif