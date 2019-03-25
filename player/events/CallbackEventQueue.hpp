#pragma once

#include "MessageQueue.hpp"
#include <functional>

using CallbackEventQueue = MessageQueue<std::function<void()>>;

CallbackEventQueue& callbackQueue();
