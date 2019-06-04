#ifndef __H_EVENTLISTENER__
#define __H_EVENTLISTENER__

#include "yellowEngine/System/Event.hpp"

class EventNotifier;

// change _notifier as vector<EventNotifier*> to listen multiple events
class EventListener
{
public:
	EventListener();
	EventListener(INotifiable* parent);
	~EventListener();

	void setParent(INotifiable* parent);
	void onListen(EventNotifier* notifier);
	void onStopListen(EventNotifier* notifier);
	void onNotify(Event event, void* sender);

private:
	EventNotifier* _notifier;
	INotifiable* _parent;
};

#endif