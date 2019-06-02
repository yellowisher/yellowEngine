#ifndef __H_EVENTNOTIFIER__
#define __H_EVENTNOTIFIER__

#include <vector>

#include "Event.hpp"

class EventListener;

class EventNotifier
{
public:
	EventNotifier();
	~EventNotifier();

	void setSender(void* sender);
	void addListener(EventListener* listener);
	void removeListener(EventListener* listener);
	void notify(Event event);

private:
	std::vector<EventListener*> _listeners;
	void* _sender;
};

#endif