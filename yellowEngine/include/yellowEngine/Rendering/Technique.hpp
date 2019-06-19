#ifndef __H_TECHNIQUE__
#define __H_TECHNIQUE__

#include <vector>

#include "yellowEngine/Component/ObjectRenderer.hpp"

namespace yellowEngine
{
	enum TechniqueType
	{
		TechniqueType_Deferred,

		Num_TechniqueType
	};

	class Technique
	{
	public:
		static Technique* getTechnique(TechniqueType type);
		static void renderScene();

		void addRenderer(ObjectRenderer* renderer);
		void removeRenderer(ObjectRenderer* renderer);

	protected:
		std::vector<ObjectRenderer*> _renderers;

		Technique() {};
		virtual ~Technique() {};
		virtual void _renderScene() = 0;

	private:
		static Technique* __techniques[Num_TechniqueType];
	};
}

#endif