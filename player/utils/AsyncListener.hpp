#pragma once

#include <functional>
#include <mutex>

#include "utils/Utilities.hpp"

class AsyncListener
{
public:
    AsyncListener(std::function<void()> finishedCallback_) : finishedCallback(finishedCallback_)
    {
    }

    template<typename R, typename... Args, typename Callable>
    std::function<R(Args&&...)> add(Callable clbk)
    {
        ++allCalls;
        return [=](Args&&... args)
        {
            std::unique_lock<std::mutex> lock{mutex};
            clbk(std::forward<Args>(args)...);
            if(++callCounter == allCalls)
            {
                finishedCallback();
            }
        };
    }

private:
    std::mutex mutex;
    std::function<void()> finishedCallback;
    int callCounter = 0;
    int allCalls = 0;
};
