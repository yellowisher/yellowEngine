#include "yellowEngine/Component/Transform.hpp"
#include "yellowEngine/Animation/Animator.hpp"


namespace yellowEngine
{
	std::vector<Animator*> Animator::__animators;


	Animator::Animator(GameObject * gameObject) :Component(gameObject)
	{
		_currentClip = nullptr;
		_frame = 0;
		__animators.push_back(this);
	}


	Animator::~Animator()
	{
		for (auto it = __animators.begin(); it != __animators.end(); ++it)
		{
			if (*it == this)
			{
				__animators.erase(it);
				return;
			}
		}
	}


	void Animator::proceedAll()
	{
		for (auto animator : __animators)
		{
			if (!animator->pause)animator->proceed();
		}
	}


	void Animator::proceed()
	{
		for (auto channel : _currentClip->_channels)
		{
			Transform* target = getTransform(channel.first.first);
			PropertyType type = channel.first.second;

			KeyFrame begin;
			KeyFrame end;

			int ei = _ends[channel.first];
			int bi = ei - 1;

			end = channel.second[];
			if (bi < 0)
			{

			}

			switch (type)
			{
				case AnimationClip::Property_PositionX:
				case AnimationClip::Property_PositionY:
				case AnimationClip::Property_PositionZ:
					break;

				case AnimationClip::Property_RotationX:
				case AnimationClip::Property_RotationY:
				case AnimationClip::Property_RotationZ:
					break;

				case AnimationClip::Property_ScaleX:
				case AnimationClip::Property_ScaleY:
				case AnimationClip::Property_ScaleZ:
					break;

				case AnimationClip::Property_Sprite:
					break;
			}
		}
	}


	Transform* Animator::getTransform(const std::string& target)
	{
		auto it = _transformCache.find(target);
		if (it == _transformCache.end())
		{
			size_t begin = 0;
			size_t end = 0;
			Transform* cursor = transform;
			while ((end = target.find('/', begin)) != std::string::npos)
			{
				int index = std::stoi(target.substr(begin, end - begin));
				cursor = cursor->getChild(index);
				begin = end + 1;
			}
			_transformCache.insert({ target, cursor });
		}
		return _transformCache[target];
	}


	void Animator::play(AnimationClip* clip, int delay)
	{
		_paused = false;
	}


	void Animator::pause()
	{
		_paused = true;
	}


	void Animator::resume()
	{
		_paused = false;
	}


	void Animator::stop()
	{
	}
}