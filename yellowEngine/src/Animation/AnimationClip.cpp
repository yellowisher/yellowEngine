#include <iostream>
#include <fstream>
#include <json/json.h>	

#include "yellowEngine/System/Game.hpp"
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

		std::ifstream document(path, std::ifstream::binary);

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

			clip->_channels.insert({ {target, prop}, {} });
			std::vector<KeyFrame>& frames = clip->_channels[{target, prop}];

			for (auto keyFrame : channel["key_frames"])
			{
				int frame = keyFrame["frame"].asInt();
				auto value = keyFrame["value"];

				switch (prop)
				{
					case Property_Position:
					case Property_Scale:
					{
						Vector3 vector3;
						vector3.x = value["x"].asFloat();
						vector3.y = value["y"].asFloat();
						vector3.z = value["z"].asFloat();
						frames.push_back(KeyFrame(frame, vector3));
						break;
					}
					case Property_Rotation:
					{
						Quaternion quaternion;
						quaternion.x = value["x"].asFloat();
						quaternion.y = value["y"].asFloat();
						quaternion.z = value["z"].asFloat();
						quaternion.w = value["w"].asFloat();
						frames.push_back(KeyFrame(frame, quaternion));
						break;
					}
				}
			}
		}

		__clipCache.insert({ path, clip });
		return clip;
	}


	void AnimationClip::saveClip(const char* path, int frameCount, std::map<Key, std::vector<KeyFrame>> channels)
	{
		Json::Value root;

		root["frame_count"] = frameCount;
		root["is_looping"] = true;

		Json::Value channelsJson;

		for (auto channel : channels)
		{
			Key key = channel.first;
			std::vector<KeyFrame> keyFrames = channel.second;

			Json::Value channelJson;
			Json::Value keyFramesJson;
			for (auto keyFrame : keyFrames)
			{
				Json::Value keyFrameJson;

				Json::Value valueJson;
				switch (key.prop)
				{
					case Property_Position:
					case Property_Scale:
					{
						Vector3 value = keyFrame.value.vector3;
						valueJson["x"] = value.x;
						valueJson["y"] = value.y;
						valueJson["z"] = value.z;
						break;
					}
					case Property_Rotation:
					{
						Quaternion value = keyFrame.value.quaternion;
						valueJson["x"] = value.x;
						valueJson["y"] = value.y;
						valueJson["z"] = value.z;
						valueJson["w"] = value.w;
						break;
					}
				}
				keyFrameJson["frame"] = keyFrame.frame;
				keyFrameJson["value"] = valueJson;

				keyFramesJson.append(keyFrameJson);
			}
			channelJson["target"] = key.transformPath + propertyToString(key.prop);
			channelJson["key_frames"] = keyFramesJson;
			channelsJson.append(channelJson);
		}

		root["channels"] = channelsJson;

		std::ofstream ofs(path, std::ifstream::binary);
		auto writer = Json::StyledStreamWriter();
		writer.write(ofs, root);
	}
}