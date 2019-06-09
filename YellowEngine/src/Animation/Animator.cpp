#include <iostream>

#include "yellowEngine/Utility/Utils.hpp"
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
			if (animator->_state == State_Playing ||
				animator->_state == State_Transitioning)
			{
				if (!animator->_paused)animator->proceed();
			}
		}
	}


	void Animator::proceed()
	{
		if (++_frame > _currentClip->_frameCount)
		{
			if (_currentClip->_isLooping)
			{
				// stop and play?
				auto clip = _currentClip;
				_currentClip = nullptr;
				play(clip);
			}
			else
			{
				stop();
			}
		}

		if (_state == State_Playing)
		{
			for (auto channel : _currentClip->_channels)
			{
				if (_frame > channel.second[_ends[channel.first]].frame)
				{
					_ends[channel.first]++;
				}

				int ei = _ends[channel.first];
				int bi = ei - 1;

				KeyFrame& begin = channel.second[bi];
				KeyFrame& end = channel.second[ei];

				float factor = (float)(_frame - begin.frame) / (float)(end.frame - begin.frame);
				apply(channel.first, Utils::lerp(begin.value, end.value, factor));
			}
		}
		else if (_state == State_Transitioning)
		{
			// frozen,	clip
			// Yes		Yes
			// Yes		No
			// No		Yes

			float factor = (float)_frame / (float)_transitionDelay;
			for (auto channel : _currentClip->_channels)
			{
				if (_frame > channel.second[_ends[channel.first]].frame)
				{
					_ends[channel.first]++;
				}
				int ei = _ends[channel.first];
				int bi = ei - 1;

				KeyFrame& begin = channel.second[bi];
				KeyFrame& end = channel.second[ei];

				float factor_ = (float)(_frame - begin.frame) / (float)(end.frame - begin.frame);
				float targetValue = Utils::lerp(begin.value, end.value, factor_);

				auto fit = _frozenValues.find(channel.first);
				if (fit != _frozenValues.end())
				{
					apply(channel.first, Utils::lerp(fit->second, targetValue, factor));
				}
				else
				{
					apply(channel.first, targetValue);
				}
			}

			for (auto frozen : _frozenValues)
			{
				if (_currentClip->_channels.find(frozen.first) == _currentClip->_channels.end())
				{
					apply(frozen.first, Utils::lerp(frozen.second, _initialValues[frozen.first], factor));
				}
			}

			// transition ended
			if (_frame == _transitionDelay)
			{
				_state = State_Playing;
				_frozenValues.clear();
			}
		}
		commit();
	}


	void Animator::play(AnimationClip* clip, int delay)
	{
		_paused = false;
		if (_currentClip == nullptr)
		{
			// play request while clean state (after terminate or first playing)
			_state = State_Playing;
			_frame = 0;
		}
		else
		{
			_state = State_Transitioning;
			_transitionDelay = delay;
			_frame = 0;

			if (_state == State_Playing)
			{
				// play request while playing
				_frozenValues.clear();
				for (auto channel : _currentClip->_channels)
				{
					_frozenValues.insert({ channel.first, getValue(channel.first) });
					_initialValues.insert({ channel.first, channel.second[0].value });
				}
			}
			else if (_state == State_Transitioning)
			{
				// play request while transitioning

				// update all frozen values to current value
				for (auto frozen : _frozenValues)
				{
					_frozenValues[frozen.first] = getValue(frozen.first);
				}

				// insert new frozen values
				for (auto channel : _currentClip->_channels)
				{
					if (_frozenValues.find(channel.first) == _frozenValues.end())
					{
						_frozenValues.insert({ channel.first, getValue(channel.first) });
						_initialValues.insert({ channel.first, channel.second[0].value });
					}
				}
			}

		}

		_currentClip = clip;
		for (auto channel : _currentClip->_channels)
		{
			_ends[channel.first] = 1;
		}
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
		_state = State_Stopped;
		if (_currentClip != nullptr)
		{
			for (auto channel : _initialValues)
			{
				apply(channel.first, channel.second);
			}
			_currentClip = nullptr;
			//_initialValues.clear();
		}
	}


	void Animator::apply(std::pair<std::string, PropertyType> pair, float value)
	{
		Transform* target = getTransform(pair.first);
		PropertyType type = pair.second;

		switch (type)
		{
			case AnimationClip::Property_PositionX:
			case AnimationClip::Property_PositionY:
			case AnimationClip::Property_PositionZ:
			{
				Vector3 position = target->position;
				int v = type - AnimationClip::Property_Position;
				position.v[v] = value;
				target->setPosition(position);
				break;
			}

			case AnimationClip::Property_RotationX:
			case AnimationClip::Property_RotationY:
			case AnimationClip::Property_RotationZ:
			{
				int v = type - AnimationClip::Property_Rotation;
				_rotations[target].v[v] = value;
				break;
			}

			case AnimationClip::Property_ScaleX:
			case AnimationClip::Property_ScaleY:
			case AnimationClip::Property_ScaleZ:
			{
				Vector3 scale = target->scale;
				int v = type - AnimationClip::Property_Scale;
				scale.v[v] = value;
				target->setScale(scale);
				break;
			}
		}
	}


	void Animator::commit()
	{
		for (auto pair : _rotations)
		{
			pair.first->setRotation(pair.second);
		}
	}


	float Animator::getValue(std::pair<std::string, PropertyType> pair)
	{
		Transform* target = getTransform(pair.first);
		PropertyType type = pair.second;

		switch (type)
		{
			case AnimationClip::Property_PositionX:
			case AnimationClip::Property_PositionY:
			case AnimationClip::Property_PositionZ:
			{
				return target->position.v[type - AnimationClip::Property_Position];
			}

			case AnimationClip::Property_RotationX:
			case AnimationClip::Property_RotationY:
			case AnimationClip::Property_RotationZ:
			{
				return _rotations[target].v[type - AnimationClip::Property_Rotation];
			}

			case AnimationClip::Property_ScaleX:
			case AnimationClip::Property_ScaleY:
			case AnimationClip::Property_ScaleZ:
			{
				target->scale.v[type - AnimationClip::Property_Scale];
			}
		}
		return 0;
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
}