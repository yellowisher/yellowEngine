#include "json/json.h"
#include "yellowEngine/System/System.hpp"
#include "yellowEngine/Animation/AnimationClip.hpp"


namespace yellowEngine
{
	AnimationClip::AnimationClip()
	{
	}


	AnimationClip::~AnimationClip()
	{
	}


	AnimationClip* AnimationClip::create(const char* path)
	{
		auto it = __clipCache.find(path);
		if (it != __clipCache.end())
		{
			return it->second;
		}

		std::string fullPath = System::getInstance()->getResourcePath(path);
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(fullPath, root))
		{
			// cannot read json file
			return nullptr;
		}

		AnimationClip* clip = new AnimationClip();
		clip->_frameRate = root["frame_rate"].asInt();
		clip->_isLooping = root["is_looping"].asBool();
		for (auto channel : root["channels"])
		{
			std::string fullTarget = channel["target"].asString();
			size_t pos = fullTarget.find_last_of('/');

			std::string target = fullTarget.substr(0, pos);
			auto propertyValue = getPropertyPair(fullTarget.substr(pos + 1));

			std::vector<KeyFrame>& frames = clip->_channels.insert({ {target, propertyValue.first}, {} }).first->second;

			for (auto keyFrame : channel["key_frames"])
			{
				int frame = keyFrame["frame"].asInt();
				if (propertyValue.second == Value_Float) frames.push_back(KeyFrame(frame, keyFrame["value"].asFloat()));
				else if (propertyValue.second == Value_String) frames.push_back(KeyFrame(frame, keyFrame["value"].asString()));
			}
		}

		__clipCache.insert({ path, clip });
		return clip;
	}
}