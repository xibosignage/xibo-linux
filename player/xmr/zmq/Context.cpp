#include "Context.hpp"
#include "common/logger/Logging.hpp"
#include "xmr/zmq/Exception.hpp"

#include <zmq.h>

Zmq::Context::Context()
{
    handle_ = zmq_ctx_new();
    if (!handle_) throw Zmq::Exception{};

    running_ = true;
}

Zmq::Context::~Context() noexcept
{
    safeTerminate();
}

void* Zmq::Context::handle()
{
    return handle_;
}

void Zmq::Context::shutdown()
{
    if (!handle_ || !running_) return;
    if (zmq_ctx_shutdown(handle_) == ErrorCode) throw Zmq::Exception{};

    running_ = false;
}

bool Zmq::Context::running() const
{
    return running_;
}

void Zmq::Context::terminate()
{
    if (safeTerminate() == ErrorCode) throw Zmq::Exception{};
}

int Zmq::Context::safeTerminate() noexcept
{
    if (!handle_) return SuccessCode;

    int error = zmq_ctx_term(handle_);
    handle_ = nullptr;
    running_ = false;
    return error;
}
