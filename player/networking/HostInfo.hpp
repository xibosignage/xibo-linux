#pragma once

#include <string>

struct HostInfo
{
    // TODO: strong type
    std::string host;
    unsigned short port;
    bool useSsl;
};
