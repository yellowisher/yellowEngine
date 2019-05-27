#include "Event.hpp"


// INotifiable
INotifiable::INotifiable()
{
}


INotifiable::~INotifiable()
{
}


// EventNotifier
EventNotifier::EventNotifier(void* sender)
{
	_sender = sender;
}


EventNotifier::~EventNotifier()
{
	for (auto listener : _listeners)
	{
		listener->onStopListen(this);
	}
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


// EventListener
EventListener::EventListener() :_notifier(nullptr)
{
}


EventListener::~EventListener()
{
	if (_notifier)
	{
		_notifier->removeListener(this);
	}
}


void EventListener::setParent(INotifiable* parent)
{
	_parent = parent;
}


void EventListener::onListen(EventNotifier* notifier)
{
	_notifier = notifier;
}


void EventListener::onStopListen(EventNotifier* notifier)
{
	_notifier = nullptr;
}


void EventListener::onNotify(Event event, void* sender)
{
	_parent->notify(event, sender);
}