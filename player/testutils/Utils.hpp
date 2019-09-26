#pragma once

#include <functional>
#include <memory>

template <typename R, typename... Args>
class FakeSlot;

template <typename R, typename... Args>
class FakeSlot<R(Args...)>
{
public:
    using Action = std::function<void()>;

    FakeSlot(const Action& action = {}) : m_action(action), m_called(std::make_shared<size_t>(0)) {}

    void operator()(Args&&... /*args*/)
    {
        ++*m_called;
        if (m_action)
        {
            m_action();
        }
    }

    bool notCalled() const
    {
        return *m_called == 0;
    }

    bool calledOnce() const
    {
        return *m_called == 1;
    }

    bool called(size_t count) const
    {
        return *m_called == count;
    }

    size_t calledCount() const
    {
        return *m_called;
    }

private:
    Action m_action;
    std::shared_ptr<size_t> m_called;
};
