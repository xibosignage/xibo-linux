#pragma once

#include <string>
#include <vector>

namespace Zmq
{
    using Channels = std::vector<std::string>;
    using MultiPartMessage = std::vector<std::string>;

    class Context;

    // ZMQ socket is not designed to work in multiple threads so does this wrapper
    class Socket
    {
    public:
        Socket(Context& context);
        ~Socket() noexcept;

        void* handle();
        void close();
        void connect(const std::string& host, const Channels& channels);
        MultiPartMessage receiveAll();

    private:
        int safeClose() noexcept;

    private:
        void* handle_ = nullptr;
    };
}
