#include "Context.hpp"

#include "networking/zmq/Exception.hpp"

Zmq::Context::Context()
{
    handle_ = zmq_ctx_new();
    if (!handle_) throw Zmq::Exception{};

    running_ = true;
}

Zmq::Context::~Context()
{
    terminate();
}

void* Zmq::Context::handle()
{
    return handle_;
}

void Zmq::Context::shutdown()
{
    std::unique_lock lock{guard_};
    if (handle_ && running_)
    {
        if (zmq_ctx_shutdown(handle_) == ErrorCode) throw Zmq::Exception{};

        running_ = false;
    }
}

bool Zmq::Context::running() const
{
    return running_;
}

void Zmq::Context::terminate()
{
    std::unique_lock lock{guard_};
    if (handle_)
    {
        if (zmq_ctx_term(handle_) == ErrorCode) throw Zmq::Exception{};

        handle_ = nullptr;
        running_ = false;
    }
}
