#pragma once

#include <atomic>

namespace Zmq
{
    // According to ZMQ docs, context can be used between multiple threads safely
    class Context
    {
    public:
        Context();
        ~Context() noexcept;

        void* handle();
        void shutdown();
        bool running() const;
        void terminate();

    private:
        int safeTerminate() noexcept;

    private:
        std::atomic_bool running_;
        void* handle_ = nullptr;
    };
}
