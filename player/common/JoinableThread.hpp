#pragma once

#include <thread>

class JoinableThread
{
public:
    template <typename Callable>
    JoinableThread(Callable startingPoint)
    {
        m_thread = std::make_unique<std::thread>(startingPoint);
    }

    ~JoinableThread()
    {
        if (m_thread && m_thread->joinable())
        {
            m_thread->join();
        }
    }

private:
    std::unique_ptr<std::thread> m_thread;
};
