#include "common/PlayerRuntimeError.hpp"
#include "common/Utils.hpp"
#include "common/types/Uri.hpp"

#include <regex>

const std::regex IpRegex(
    "(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])");

template <>
std::string Utils::toString(Uri::Host::Type val)
{
    switch (val)
    {
        case Uri::Host::Type::DNS: return "DNS";
        case Uri::Host::Type::IP: return "IP";
    }

    return "unknown";
}

Uri::Host Uri::Host::fromString(const std::string& host)
{
    if (host.empty()) throw PlayerRuntimeError{"Uri::Host", "Invalid"};

    std::smatch baseMatch;
    if (std::regex_match(host, baseMatch, IpRegex))
    {
        return Uri::Host{host, Type::IP};
    }
    else
    {
        return Uri::Host{host, Type::DNS};
    }
}

Uri::Host::Type Uri::Host::type() const
{
    return type_;
}

Uri::Host::Host(const std::string& host, Uri::Host::Type type) : StrongType{host}, type_(type) {}

bool operator==(const Uri::Host& first, const Uri::Host& second)
{
    return static_cast<std::string>(first) == static_cast<std::string>(second);
}

bool operator!=(const Uri::Host& first, const Uri::Host& second)
{
    return !(first == second);
}
