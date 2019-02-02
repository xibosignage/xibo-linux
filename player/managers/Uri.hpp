#pragma once

#include <iostream>

struct Uri
{
    enum class Scheme
    {
        HTTP,
        HTTPS,
        File
    } scheme;

    struct Authority
    {
        std::string host;

        enum class HostType
        {
            DNS,
            IP
        } hostType;

        unsigned short port = 0;

    } authority;

    std::string path;
};

inline std::ostream& operator<<(std::ostream& out, const Uri& uri)
{
   return out << "Scheme: " <<  static_cast<int>(uri.scheme)
              << " HostType: " <<  static_cast<int>(uri.authority.hostType)
              << " Host: " << uri.authority.host
              << " Port: " << uri.authority.port
              << " Target: " << uri.path;
}
