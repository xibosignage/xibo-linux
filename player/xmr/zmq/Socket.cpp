#include "Socket.hpp"
#include "common/logger/Logging.hpp"
#include "xmr/zmq/Context.hpp"
#include "xmr/zmq/Exception.hpp"
#include "xmr/zmq/Message.hpp"

#include <cassert>
#include <zmq.h>

Zmq::Socket::Socket(Context& context)
{
    handle_ = zmq_socket(context.handle(), ZMQ_SUB);
    if (!handle_) throw Zmq::Exception{};
}

Zmq::Socket::~Socket() noexcept
{
    safeClose();
}

void* Zmq::Socket::handle()
{
    return handle_;
}

void Zmq::Socket::close()
{
    if (safeClose() == ErrorCode) throw Zmq::Exception{};
}

int Zmq::Socket::safeClose() noexcept
{
    if (!handle_) return SuccessCode;

    int error = zmq_close(handle_);
    handle_ = nullptr;
    return error;
}

void Zmq::Socket::connect(const std::string& host, const Channels& channels)
{
    if (!handle_) return;
    if (zmq_connect(handle_, host.c_str()) == ErrorCode) throw Zmq::Exception{};

    for (auto&& channel : channels)
    {
        if (zmq_setsockopt(handle_, ZMQ_SUBSCRIBE, channel.c_str(), channel.size()) == ErrorCode)
            throw Zmq::Exception{};
    }
    int linger = 0;  // how long pending messages will live after zmq_close
    if (zmq_setsockopt(handle_, ZMQ_LINGER, &linger, sizeof(linger)) == ErrorCode) throw Zmq::Exception{};
}

Zmq::MultiPartMessage Zmq::Socket::receiveAll()
{
    if (!handle_) return {};

    MultiPartMessage composedMessage;

    while (1)
    {
        Message message;
        message.receiveData(*this);

        std::string data{message.data(), message.size()};
        composedMessage.emplace_back(data);

        if (!message.more()) break;
    }

    return composedMessage;
}
