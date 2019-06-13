#ifndef __H_EVENT__
#define __H_EVENT__

#include <vector>

namespace yellowEngine
{
	enum Event
	{
		Event_TransformChanged,
	};

	// use template rather than void*?
	class INotifiable
	{
	public:
		INotifiable() {};
		virtual ~INotifiable() {};
		virtual void notify(Event event, void* sender) = 0;
	};
}

#endif