#ifndef __H_ANIMATOR__
#define __H_ANIMATOR__

#include <string>
#include <vector>
#include <map>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Animation/AnimationClip.hpp"

namespace yellowEngine
{
	class Animator : public Component
	{
		using Value = AnimationClip::Value;
		using KeyFrame = AnimationClip::KeyFrame;
		using PropertyType = AnimationClip::PropertyType;

	public:
		static void proceedAll();

		Animator(GameObject* gameObject);
		~Animator();

		void play(AnimationClip* clip, int delay = 20);
		void pause();
		void resume();
		void stop();

	private:
		static std::vector<Animator*> __animators;

		void proceed();
		Transform* getTransform(const std::string& target);

		int _frame;
		bool _paused;
		AnimationClip* _currentClip;

		// cache for child transform; finding transform by name in every frame might be expansive
		std::map<std::string, Transform*> _transformCache;
		
		// store base value of {target, property}
		std::map<std::pair<std::string, PropertyType>, Value> _baseValues;

		std::map<std::pair<std::string, PropertyType>, int> _ends;
	};
}

#endif