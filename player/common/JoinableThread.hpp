#pragma once

#include <thread>

class JoinableThread
{
public:
    template <typename Callable>
    JoinableThread(Callable startingPoint)
    {
        thread_ = std::make_unique<std::thread>(startingPoint);
    }

    ~JoinableThread()
    {
        if (thread_ && thread_->joinable())
        {
            thread_->join();
        }
    }

private:
    std::unique_ptr<std::thread> thread_;
};
