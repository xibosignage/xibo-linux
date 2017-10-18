#ifndef EVENTSQUEUE_HPP
#define EVENTSQUEUE_HPP

#include <wx/event.h>
#include <iostream>
#include <vector>


using EventCallback = std::function<void(wxPaintEvent&)>;


struct Event
{
    EventCallback callback;
    int priority;
};

class EventsQueue : public wxEvtHandler
{
public:
    EventsQueue() = default;

    void AddEvent(EventCallback callback, int creationOrder, int priority);

private:
    int waitForNextPaint(int ms = 100);
    void paint();

private:
    std::vector<Event> m_eventsQueue;

};

#endif // EVENTSQUEUE_HPP
