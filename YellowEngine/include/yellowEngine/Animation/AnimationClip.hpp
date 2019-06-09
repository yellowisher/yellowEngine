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
		enum ValueType
		{
			Value_Float,
			Value_String
		};

		enum PropertyType
		{
			Property_Position = 0,
			Property_PositionX = 0,
			Property_PositionY,
			Property_PositionZ,

			Property_Rotation=3,
			Property_RotationX = 3,
			Property_RotationY,
			Property_RotationZ,

			Property_Scale =6,
			Property_ScaleX = 6,
			Property_ScaleY,
			Property_ScaleZ,

			Property_Sprite
		};

		union Value
		{
			Value(const Value& copy) {}
			Value(float fv) :floatValue(fv) {};
			Value(const std::string& sv) :stringValue(sv) {};
			~Value() {}

			float floatValue;
			const std::string stringValue;
		};

		struct KeyFrame
		{
			KeyFrame(const KeyFrame& copy) :frame(copy.frame), data(copy.data) {}
			KeyFrame(int frame, float value) :frame(frame), data(value) {}
			KeyFrame(int frame, const std::string& value) :frame(frame), data(value) {}
			~KeyFrame() {}

			int frame;
			Value data;
		};

		static std::pair<PropertyType, ValueType> getPropertyPair(std::string string)
		{
			static std::map<std::string, std::pair<PropertyType, ValueType>> __types =
			{
				{"position.x", {Property_PositionX, Value_Float}},
				{"position.y", {Property_PositionY, Value_Float}},
				{"position.z", {Property_PositionZ, Value_Float}},
				{"rotation.x", {Property_RotationX, Value_Float}},
				{"rotation.y", {Property_RotationY, Value_Float}},
				{"rotation.z", {Property_RotationZ, Value_Float}},
				{"scale.x", {Property_ScaleX, Value_Float}},
				{"scale.y", {Property_ScaleY, Value_Float}},
				{"scale.z", {Property_ScaleZ, Value_Float}},
				{"sprite", {Property_Sprite, Value_String}},
			};

			return __types[string];
		}
		static std::map<std::string, AnimationClip*> __clipCache;

		AnimationClip();
		~AnimationClip();

		int _frameRate;
		bool _isLooping;
		// mapping {target, property} to {key frames}
		std::map<std::pair<std::string, PropertyType>, std::vector<KeyFrame>> _channels;
	};
}

#endif