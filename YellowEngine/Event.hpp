#ifndef __H_EVENT__
#define __H_EVENT__

#include <vector>

enum Event
{
	Event_TransformChanged,
};

class EventListener;

// use template rather than void*?
class INotifiable
{
public:
	INotifiable();
	virtual ~INotifiable();
	virtual void notify(Event event, void* sender) = 0;
};


class EventNotifier
{
public:
	EventNotifier(void* sender);
	~EventNotifier();

	void addListener(EventListener* listener);
	void removeListener(EventListener* listener);
	void notify(Event event);

private:
	std::vector<EventListener*> _listeners;
	void* _sender;
};


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