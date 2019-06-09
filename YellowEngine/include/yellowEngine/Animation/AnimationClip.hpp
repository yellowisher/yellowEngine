#ifndef __H_ANIMATIONCLIP__
#define __H_ANIMATIONCLIP__

#include <vector>
#include <string>
#include <map>

namespace yellowEngine
{
	class AnimationClip
	{
		friend class Animator;

	public:
		static AnimationClip* create(const char* path);

	private:
		enum PropertyType
		{
			Property_Position = 0,
			Property_PositionX = 0,
			Property_PositionY,
			Property_PositionZ,

			Property_Rotation = 3,
			Property_RotationX = 3,
			Property_RotationY,
			Property_RotationZ,

			Property_Scale =6,
			Property_ScaleX = 6,
			Property_ScaleY,
			Property_ScaleZ,
		};

		struct KeyFrame
		{
			KeyFrame(int frame, float value) :frame(frame), value(value) {}
			~KeyFrame() {}

			int frame;
			float value;
		};

		static PropertyType getProperty(std::string string)
		{
			static std::map<std::string, PropertyType> __types =
			{
				{"position.x", {Property_PositionX}},
				{"position.y", {Property_PositionY}},
				{"position.z", {Property_PositionZ}},
				{"rotation.x", {Property_RotationX}},
				{"rotation.y", {Property_RotationY}},
				{"rotation.z", {Property_RotationZ}},
				{"scale.x", {Property_ScaleX}},
				{"scale.y", {Property_ScaleY}},
				{"scale.z", {Property_ScaleZ}}
			};

			return __types[string];
		}
		static std::map<std::string, AnimationClip*> __clipCache;

		AnimationClip();
		~AnimationClip();

		int _frameCount;
		bool _isLooping;
		// mapping {target, property} to {key frames}
		std::map<std::pair<std::string, PropertyType>, std::vector<KeyFrame>> _channels;
	};
}

#endif