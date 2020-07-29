#include "Exception.hpp"

#include <zmq.h>

Zmq::Exception::Exception() : error_(errno) {}

int Zmq::Exception::code() const
{
    return error_;
}

const char* Zmq::Exception::what() const noexcept
{
    return zmq_strerror(error_);
}
