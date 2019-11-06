#pragma once

#include <exception>
#include <zmq.h>

const int ErrorCode = -1;

namespace Zmq
{
    class Exception : public std::exception
    {
    public:
        Exception();

        int code() const;
        const char* what() const noexcept;

    private:
        int error_;
    };
}
