#pragma once

#include <string>
#include <regex>
#include <boost/optional/optional.hpp>

#include "Uri.hpp"

class UriParser
{
public:
    Uri parse(const std::string& rawUri);
    Uri::Scheme getScheme(const std::string& scheme);
    Uri::Authority::HostType getHostType(const std::string& host);
    boost::optional<unsigned short> getPortNumber(Uri::Scheme scheme, const std::string& port);

private:
    std::smatch parseInternal(const std::string& rawUri);

};
