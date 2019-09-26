#pragma once

#include <boost/optional/optional.hpp>
#include <regex>
#include <string>

#include "Uri.hpp"

class UriParser
{
public:
    static Uri parse(const std::string& rawUri);
    static Uri::Scheme getScheme(const std::string& scheme);
    static Uri::Authority::HostType getHostType(const std::string& host);

private:
    static boost::optional<std::string> getUserInfo(const std::string& userinfo);
    static boost::optional<unsigned short> getPortNumber(const std::string& port);
    static std::string getPath(const std::string& target, const std::string& query, const std::string& fragment);

    static std::smatch parseInternal(const std::string& rawUri);
};
