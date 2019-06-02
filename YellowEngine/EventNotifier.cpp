#include "EventListener.hpp"
#include "EventNotifier.hpp"


EventNotifier::EventNotifier() :_sender(nullptr)
{
}


EventNotifier::~EventNotifier()
{
	for (auto listener : _listeners)
	{
		listener->onStopListen(this);
	}
}


void EventNotifier::setSender(void* sender)
{
	_sender = sender;
}


void EventNotifier::addListener(EventListener* listener)
{
	_listeners.push_back(listener);
	listener->onListen(this);
}


void EventNotifier::removeListener(EventListener* listener)
{
	for (auto it = _listeners.begin(); it != _listeners.end(); ++it)
	{
		if (*it == listener)
		{
			_listeners.erase(it);
			return;
		}
	}
}


void EventNotifier::notify(Event event)
{
	for (auto listener : _listeners)
	{
		listener->onNotify(event, _sender);
	}
}