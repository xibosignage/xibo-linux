#pragma once

#include <string>
#include <vector>
#include <zmq.h>

using Channels = std::vector<std::string>;
using MultiPartMessage = std::vector<std::string>;

namespace Zmq
{
    class Context;

    // ZMQ socket is not designed to work in multiple threads so does this wrapper
    class Socket
    {
    public:
        Socket(Context& context);
        ~Socket();

        void* handle();
        void close();
        void connect(const std::string& host, const Channels& channels);
        MultiPartMessage receiveAll();

    private:
        void* handle_ = nullptr;
    };
}
