#pragma once

#include "MessageQueue.hpp"
#include <functional>

using CallbackGlobalQueue = MessageQueue<std::function<void()>>;

CallbackGlobalQueue& callbackQueue();
