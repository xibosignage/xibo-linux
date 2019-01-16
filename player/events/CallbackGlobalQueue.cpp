#include "CallbackGlobalQueue.hpp"

CallbackGlobalQueue& callbackQueue()
{
    static CallbackGlobalQueue queue;
    return queue;
}
