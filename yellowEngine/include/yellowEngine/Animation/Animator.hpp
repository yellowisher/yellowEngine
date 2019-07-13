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
		BEGIN_COMPONENT(Animator)
		END_COMPONENT

		using Key = AnimationClip::Key;
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

		// should be private and expose as friend for editor
		Transform* getTransform(const std::string& target);
		void gotoFrame(float frame);
		void setClip(AnimationClip* clip);

		void setSpeed(float speed);
		float getSpeed();

	private:
		enum State
		{
			State_Stopped,
			State_Playing,
			State_Transitioning
		};

		static std::vector<Animator*> __animators;

		void proceed();
		Value lerp(Value a, Value b, float factor, PropertyType type);
		void apply(Key pair, Value value);
		Value getValue(Key pair);

		float _speed;
		State _state;
		bool _paused;
		float _frame;
		int _transitionDelay;
		AnimationClip* _currentClip;
		
		// cache for child transform; finding transform by name in every frame might be expansive
		std::map<std::string, Transform*> _transformCache;
		
		// current end points
		std::map<Key, int> _ends;

		// frozen value for transition
		std::map<Key, Value> _frozenValues;
		std::map<Key, Value> _initialValues;
	};
}

#endif