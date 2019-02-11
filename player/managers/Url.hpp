#pragma once

#include "utils/Logger.hpp"

struct Url
{
    enum class Scheme
    {
        HTTP,
        HTTPS
    } scheme;

    struct Host
    {
        std::string host;

        enum class Type
        {
            DNS,
            IP
        } type;

        operator std::string() const
        {
            return host;
        }

    } host;

    unsigned short port;
    std::string target;

    void print() const
    {
        Log::debug("Scheme: {} HostType: {} Host: {} Port: {} Target: {}", static_cast<int>(scheme), static_cast<int>(host.type), host.host, port, target);
    }
};
