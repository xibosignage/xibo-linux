#pragma once

#include <deque>
#include <mutex>

template<typename MessageType>
class MessageQueue
{
public:
    bool empty() const
    {
        return m_queue.empty();
    }

    void add(const MessageType& message)
    {
        std::unique_lock<std::mutex> lock{m_queueMutex};

        m_queue.push_back(message);
    }

    void clear()
    {
        m_queue.clear();
    }

    [[nodiscard]] MessageType pop()
    {
        std::unique_lock<std::mutex> lock{m_queueMutex};

        auto message = m_queue.front();
        m_queue.pop_front();
        return message;
    }

private:
    std::deque<MessageType> m_queue;
    std::mutex m_queueMutex;

};
