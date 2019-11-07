#include "Message.hpp"
#include "common/logger/Logging.hpp"
#include "xmr/zmq/Exception.hpp"
#include "xmr/zmq/Socket.hpp"

Zmq::Message::Message()
{
    zmq_msg_init(&handle_);
}

Zmq::Message::~Message() noexcept
{
    safeClose();
}

char* Zmq::Message::data()
{
    return reinterpret_cast<char*>(zmq_msg_data(&handle_));
}

size_t Zmq::Message::size() const
{
    return zmq_msg_size(&handle_);
}

void Zmq::Message::receiveData(Zmq::Socket& socket)
{
    if (zmq_msg_recv(&handle_, socket.handle(), 0) == ErrorCode) throw Zmq::Exception{};
}

void Zmq::Message::close()
{
    if (safeClose() == ErrorCode) throw Zmq::Exception{};
}

int Zmq::Message::safeClose() noexcept
{
    return zmq_msg_close(&handle_);
}

bool Zmq::Message::more() const
{
    return zmq_msg_more(&handle_);
}
