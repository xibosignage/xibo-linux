#pragma once

#include <memory>

template<typename R, typename... Args>
class FakeSlot;

template<typename R, typename... Args>
class FakeSlot<R(Args...)>
{
public:
    FakeSlot() :
        m_called(std::make_shared<size_t>(0))
    {
    }

    void operator()(Args&&... /*args*/)
    {
        ++*m_called;
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
    std::shared_ptr<size_t> m_called;

};
