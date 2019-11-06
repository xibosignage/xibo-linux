#pragma once

#include <atomic>
#include <mutex>
#include <zmq.h>

namespace Zmq
{
    // According to ZMQ docs, context can be used between multiple threads safely
    class Context
    {
    public:
        Context();
        ~Context();

        void* handle();
        void shutdown();
        bool running() const;
        void terminate();

    private:
        std::mutex guard_;
        std::atomic_bool running_;
        void* handle_ = nullptr;
    };
}
