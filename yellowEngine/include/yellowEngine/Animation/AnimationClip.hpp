#ifndef __H_ANIMATIONCLIP__
#define __H_ANIMATIONCLIP__

#include <vector>
#include <string>
#include <map>

#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Math/Quaternion.hpp"

namespace yellowEngine
{
	class AnimationClip
	{
		friend class Model;
		friend class Animator;

	public:
		static AnimationClip* create(const char* path);

	private:
		enum PropertyType
		{
			Property_Position,
			Property_Rotation,
			Property_Scale
		};

		struct Value
		{
			Value() {};
			Value(const Value& copy) { quaternion = copy.quaternion; }
			Value& operator=(const Value& other) { quaternion = other.quaternion; return *this; }
			~Value() {};

			Value(const Vector3 vector3) :vector3(vector3) {}
			Value(const Quaternion quaternion) :quaternion(quaternion) {}
			union
			{
				Quaternion quaternion;
				Quaternion rotation;

				Vector3 vector3;
				Vector3 position;
				Vector3 scale;
			};
		};

		struct KeyFrame
		{
			//KeyFrame() {};
			KeyFrame(int frame, Vector3 value) :frame(frame), value(value) {}
			KeyFrame(int frame, Quaternion value) :frame(frame), value(value) {}
			~KeyFrame() {}

			int frame;
			Value value;
		};

		/*
			instead of bunch of std::pair<>, use own structure
			nothing but some syntax sugar
		*/
		struct Key
		{
			std::string transformPath;
			PropertyType prop;

			bool operator<(const Key& other) const
			{
				if (transformPath == other.transformPath)
				{
					return prop < other.prop;
				}
				return transformPath < other.transformPath;
			}
		};

		static PropertyType getProperty(std::string string)
		{
			static std::map<std::string, PropertyType> __types =
			{
				{"position", {Property_Position}},
				{"rotation", {Property_Rotation}},
				{"scale", {Property_Scale}},
			};

			return __types[string];
		}
		static std::map<std::string, AnimationClip*> __clipCache;

		AnimationClip();
		~AnimationClip();

		int _frameCount;
		bool _isLooping;
		// mapping {target, property} to {key frames}
		std::map<Key, std::vector<KeyFrame>> _channels;
	};
}

#endif