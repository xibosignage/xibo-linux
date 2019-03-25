#include "CallbackEventQueue.hpp"

CallbackEventQueue& callbackQueue()
{
    static CallbackEventQueue queue;
    return queue;
}
