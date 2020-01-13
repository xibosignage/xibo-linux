#pragma once

#include <zmq.h>

namespace Zmq
{
    class Socket;

    class Message
    {
    public:
        Message();
        ~Message() noexcept;

        char* data();
        size_t size() const;
        void receiveData(Socket& socket);
        void close();
        bool more() const;

    private:
        int safeClose() noexcept;

    private:
        zmq_msg_t handle_;
    };

}
