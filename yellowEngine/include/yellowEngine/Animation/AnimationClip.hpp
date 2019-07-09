#ifndef __H_ANIMATIONCLIP__
#define __H_ANIMATIONCLIP__

#include <vector>
#include <string>
#include <map>

#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Math/Quaternion.hpp"


// TODO:: currently all fields are exposed as public
// because in editor, animation handling window is not class, just a function.
// So change editor window as class and just friend keyword later.
namespace yellowEngine
{
	class AnimationClip
	{
		friend class Model;
		friend class Animator;

	public:
		enum PropertyType
		{
			Property_Position,
			Property_Rotation,
			Property_Scale
		};

		struct Value
		{
			Value() {};
			Value(const Value& copy) { vector3 = copy.vector3; }
			Value& operator=(const Value& other) { vector3 = other.vector3; return *this; }
			~Value() {};

			Value(const Vector3 vector3) :vector3(vector3) {}
			union
			{
				Vector3 vector3;
				Vector3 position;
				Vector3 rotation;
				Vector3 scale;
			};
		};

		static PropertyType getProperty(std::string string)
		{
			static std::map<std::string, PropertyType> __types =
			{
				{"Position", {Property_Position}},
				{"Rotation", {Property_Rotation}},
				{"Scale", {Property_Scale}},
			};

			return __types[string];
		}
		
		static std::string propertyToString(PropertyType prop)
		{
			static std::string __strings[] = {
				"Position",
				"Rotation",
				"Scale"
			};

			return __strings[prop];
		}

		struct KeyFrame
		{
			KeyFrame() {};
			KeyFrame(int frame, Vector3 value) :frame(frame), value(value) {}
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

			bool operator==(const Key& other) const
			{
				return transformPath == other.transformPath && prop == other.prop;
			}
		};

		static AnimationClip* create(const char* path);
		static void saveClip(const char* path, AnimationClip* clip);

		AnimationClip();
		~AnimationClip();

		int _frameCount;
		bool _isLooping;

		// mapping {target, property} to {key frames}
		std::map<Key, std::vector<KeyFrame>> _channels;

	private:
		static std::map<std::string, AnimationClip*> __clipCache;
	};
}

#endif