#include <iostream>
#include <fstream>

#include "json/json.h"
#include "yellowEngine/System/System.hpp"
#include "yellowEngine/Animation/AnimationClip.hpp"


namespace yellowEngine
{
	std::map<std::string, AnimationClip*> AnimationClip::__clipCache;


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
		std::ifstream document(fullPath, std::ifstream::binary);

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(document, root))
		{
			std::cout << "Json parseing failed" << std::endl;
			return nullptr;
		}

		AnimationClip* clip = new AnimationClip();
		clip->_frameCount = root["frame_count"].asInt();
		clip->_isLooping = root["is_looping"].asBool();
		for (auto channel : root["channels"])
		{
			std::string fullTarget = channel["target"].asString();
			size_t pos = fullTarget.find_last_of('/');

			std::string target = fullTarget.substr(0, pos + 1);
			auto prop = getProperty(fullTarget.substr(pos + 1));

			std::vector<KeyFrame>& frames = clip->_channels.insert({ {target, prop}, {} }).first->second;

			for (auto keyFrame : channel["key_frames"])
			{
				int frame = keyFrame["frame"].asInt();
				frames.push_back(KeyFrame(frame, keyFrame["value"].asFloat()));
			}
		}

		__clipCache.insert({ path, clip });
		return clip;
	}
}