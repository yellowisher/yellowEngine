#include <iostream>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Component/Transform.hpp"
#include "yellowEngine/Animation/Animator.hpp"


namespace yellowEngine
{
	COMPONENT_IMPL(Animator)

	std::vector<Animator*> Animator::__animators;


	Animator::Animator(GameObject* gameObject) :Component(gameObject)
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
			if (animator->_state == State_Playing || animator->_state == State_Transitioning)
			{
				if (!animator->_paused)animator->proceed();
			}
		}
	}


	AnimationClip::Value Animator::lerp(Value a, Value b, float factor, PropertyType type)
	{
		Value result;
		switch (type)
		{
			case AnimationClip::Property_Position:
			case AnimationClip::Property_Scale:
			case AnimationClip::Property_Rotation:
				result.vector3 = Vector3::lerp(a.vector3, b.vector3, factor);
				break;
		}
		return result;
	}


	void Animator::proceed()
	{
		if (++_frame >= _currentClip->_frameCount)
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
			for (auto keyChannelPair : _currentClip->_channels)
			{
				auto key = keyChannelPair.first;
				auto& channel = keyChannelPair.second;

				if (_frame > channel[_ends[key]].frame)
				{
					_ends[key]++;
				}

				int ei = _ends[key];
				int bi = ei - 1;

				KeyFrame& begin = channel[bi];
				KeyFrame& end = channel[ei];

				float factor = (float)(_frame - begin.frame) / (float)(end.frame - begin.frame);

				// maybe make structure for channel pair would be better
				apply(key, lerp(begin.value, end.value, factor, key.prop));
			}
		}
		else if (_state == State_Transitioning)
		{
			/*
				Some properties are exist in frozen properties while some are not (and/or in current clip)
				So handle 4 case according their existence
				
				frozen value,	current clip
				Exsit			Exsit
				Exsit			Not Exsit
				Not Exsit		Exsit
				Not Exsit		Not Exist (free case; Horay!)
			*/

			float factor = (float)_frame / (float)_transitionDelay;
			for (auto keyChannelPair : _currentClip->_channels)
			{
				auto key = keyChannelPair.first;
				auto& channel = keyChannelPair.second;

				if (_frame > channel[_ends[key]].frame)
				{
					_ends[key]++;
				}
				int ei = _ends[key];
				int bi = ei - 1;

				KeyFrame& begin = channel[bi];
				KeyFrame& end = channel[ei];

				float factor_ = (float)(_frame - begin.frame) / (float)(end.frame - begin.frame);
				Value targetValue = lerp(begin.value, end.value, factor_, key.prop);

				auto fit = _frozenValues.find(key);
				if (fit != _frozenValues.end())
				{
					// should match euler angle sides of frozan value and target value
					// a better aproach would be adjust them only at transition start,
					// but naive assumption that transition might not be that long is applied
					// FIXME:: adjust frozen value only at frozening
					if (key.prop == AnimationClip::Property_Rotation)
					{
						for (int v = 0; v < 3; v++)
						{
							if (Utils::abs(fit->second.vector3.v[v] - targetValue.vector3.v[v]) > 180.0f)
							{
								if (fit->second.vector3.v[v] > 0)
								{
									fit->second.vector3.v[v] -= 360.0f;
								}
								else
								{
									fit->second.vector3.v[v] += 360.0f;
								}
							}
						}
					}
					
					apply(key, lerp(fit->second, targetValue, factor, key.prop));
				}
				else
				{
					apply(key, targetValue);
				}
			}

			for (auto frozen : _frozenValues)
			{
				if (_currentClip->_channels.find(frozen.first) == _currentClip->_channels.end())
				{
					apply(frozen.first, lerp(frozen.second, _initialValues[frozen.first], factor, frozen.first.prop));
				}
			}

			// transition ended
			if (_frame == _transitionDelay)
			{
				_state = State_Playing;
				_frozenValues.clear();
			}
		}
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
			_state = State_Transitioning;
			_transitionDelay = delay;
			_frame = 0;
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


	void Animator::gotoFrame(int frame)
	{
		for (auto channel : _currentClip->_channels)
		{
			int bi = 0;
			for (bi = 0; bi < channel.second.size() - 2; bi++)
			{
				if (channel.second[bi + 1].frame > frame)
				{
					break;
				}
			}

			KeyFrame& begin = channel.second[bi];
			KeyFrame& end = channel.second[bi + 1];

			float factor = (float)(frame - begin.frame) / (float)(end.frame - begin.frame);

			// maybe make structure for channel pair would be better
			apply(channel.first, lerp(begin.value, end.value, factor, channel.first.prop));
		}
	}


	void Animator::setClip(AnimationClip* clip)
	{
		_currentClip = clip;
		_frame = 0;
		_state = State_Stopped;
	}


	void Animator::apply(Key key, Value value)
	{
		switch (key.prop)
		{
			case AnimationClip::Property_Position:
			{
				getTransform(key.transformPath)->setPosition(value.position);
				break;
			}

			case AnimationClip::Property_Rotation:
			{
				getTransform(key.transformPath)->setRotation(value.rotation);
				break;
			}

			case AnimationClip::Property_Scale:
			{
				getTransform(key.transformPath)->setScale(value.scale);
				break;
			}
		}
	}


	AnimationClip::Value Animator::getValue(Key key)
	{
		switch (key.prop)
		{
			case AnimationClip::Property_Position:
				return Value(getTransform(key.transformPath)->position);

			case AnimationClip::Property_Rotation:
				return Value(getTransform(key.transformPath)->eulerRotation);

			case AnimationClip::Property_Scale:
				return Value(getTransform(key.transformPath)->scale);

			default:
				return Value();
		}
	}


	Transform* Animator::getTransform(const std::string& target)
	{
		auto it = _transformCache.find(target);
		if (it == _transformCache.end())
		{
			// name version
			size_t begin = 0;
			size_t end = 0;
			Transform* cursor = transform;
			while ((end = target.find('/', begin)) != std::string::npos)
			{
				std::string word = target.substr(begin, end - begin);
				if (word == "..")
				{
					cursor = cursor->getParent();
				}
				else
				{
					cursor = cursor->findChild(word);
				}
				begin = end + 1;
			}
			_transformCache.insert({ target, cursor });

			/*	index version
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
			*/
		}
		return _transformCache[target];
	}
}