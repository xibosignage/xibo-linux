#include "Socket.hpp"

#include "networking/zmq/Context.hpp"
#include "networking/zmq/Exception.hpp"

Zmq::Socket::Socket(Context& context)
{
    handle_ = zmq_socket(context.handle(), ZMQ_SUB);
    if (!handle_) throw Zmq::Exception{};
}

Zmq::Socket::~Socket()
{
    close();
}

void* Zmq::Socket::handle()
{
    return handle_;
}

void Zmq::Socket::close()
{
    if (handle_)
    {
        int error = zmq_close(handle_);
        if (error == ErrorCode) throw Zmq::Exception{};

        handle_ = nullptr;
    }
}

void Zmq::Socket::connect(const std::string& host, const Channels& channels)
{
    if (zmq_connect(handle_, host.c_str()) == ErrorCode) throw Zmq::Exception{};

    for (auto&& channel : channels)
    {
        if (zmq_setsockopt(handle_, ZMQ_SUBSCRIBE, channel.c_str(), channel.size()) == ErrorCode)
            throw Zmq::Exception{};
    }
    int linger = 0;  // how long pending messages will live after zmq_close
    if (zmq_setsockopt(handle_, ZMQ_LINGER, &linger, sizeof(linger)) == ErrorCode) throw Zmq::Exception{};
}

MultiPartMessage Zmq::Socket::receiveAll()
{
    MultiPartMessage composedMessage;

    while (1)
    {
        zmq_msg_t message;
        zmq_msg_init(&message);
        if (zmq_msg_recv(&message, handle_, 0) == ErrorCode) throw Zmq::Exception{};

        std::string data{static_cast<char*>(zmq_msg_data(&message)), zmq_msg_size(&message)};
        composedMessage.emplace_back(data);

        if (zmq_msg_close(&message) == ErrorCode) throw Zmq::Exception{};
        if (!zmq_msg_more(&message)) break;
    }

    return composedMessage;
}
