#include "EventsQueue.hpp"


void EventsQueue::AddEvent(EventCallback callback, int creationOrder, int priority)
{
    Event ev{callback, priority};
    m_eventsQueue.push_back(ev);

    std::cout << creationOrder << std::endl;
    if(creationOrder < 4)
    {
        int result = waitForNextPaint();
        if(result == -1)
            paint();
    }
    else
    {
        paint();
    }
}

int EventsQueue::waitForNextPaint(int ms)
{
    return 0;
}

void EventsQueue::paint()
{
    std::cout << "here" << std::endl;

    std::sort(m_eventsQueue.begin(), m_eventsQueue.end(), [=](const Event& first,  const Event& second){
        return first.priority < second.priority;
    });

    for(auto&& event : m_eventsQueue)
    {
        //event.callback();
    }

    m_eventsQueue.clear();
}
