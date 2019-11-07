#pragma once

#include <exception>

const int ErrorCode = -1;
const int SuccessCode = 0;

namespace Zmq
{
    class Exception : public std::exception
    {
    public:
        Exception();

        int code() const;
        const char* what() const noexcept override;

    private:
        int error_;
    };
}
