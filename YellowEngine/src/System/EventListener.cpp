#include "yellowEngine/System/EventNotifier.hpp"
#include "yellowEngine/System/EventListener.hpp"


EventListener::EventListener() :_notifier(nullptr)
{
}


EventListener::EventListener(INotifiable* parent) : _parent(parent)
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