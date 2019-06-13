#ifndef __H_ANIMATOR__
#define __H_ANIMATOR__

#include <string>
#include <vector>
#include <map>

#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Animation/AnimationClip.hpp"

namespace yellowEngine
{
	class Animator : public Component
	{
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
		enum State
		{
			State_Stopped,
			State_Playing,
			State_Transitioning
		};

		static std::vector<Animator*> __animators;

		void proceed();
		void apply(std::pair<std::string, PropertyType> pair, float value);
		void commit();
		float getValue(std::pair<std::string, PropertyType> pair);
		Transform* getTransform(const std::string& target);

		State _state;
		bool _paused;
		int _frame;
		int _transitionDelay;
		AnimationClip* _currentClip;
		
		// to handle euler angle seperated, store rotation value and apply it when commit()
		std::map<Transform*, Vector3> _rotations;

		// cache for child transform; finding transform by name in every frame might be expansive
		std::map<std::string, Transform*> _transformCache;
		
		// current end points
		std::map<std::pair<std::string, PropertyType>, int> _ends;

		// frozen value for transition
		std::map<std::pair<std::string, PropertyType>, float> _frozenValues;
		std::map<std::pair<std::string, PropertyType>, float> _initialValues;
	};
}

#endif