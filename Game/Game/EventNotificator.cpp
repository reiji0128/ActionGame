#include "EventNotificator.h"

EventNotificator::EventNotificator()
{
}

EventNotificator::~EventNotificator()
{
}

void EventNotificator::AddEvent(const EventType addEventType, const std::function<void(int)> addFunction)
{
	// map‚É‘}“ü
	eventList.insert(std::make_pair(addEventType,addFunction));
}

void EventNotificator::Notificate(const EventType eventType)
{
	/*for (auto iter = eventList.begin();iter != eventList.end(); ++iter)
	{
		if (iter->first == eventType)
		{
			iter->second;
		}
	}*/

	auto iter = eventList.find(eventType);
	iter->second;
}
