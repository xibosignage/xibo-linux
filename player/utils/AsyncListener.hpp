#pragma once

#include <functional>
#include <mutex>
#include <atomic>

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
    std::atomic<int> callCounter = 0;
    int allCalls = 0;
};
