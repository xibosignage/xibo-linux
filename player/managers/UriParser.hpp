#pragma once

#include <string>
#include <regex>
#include <boost/optional/optional.hpp>

#include "Uri.hpp"

class UriParser
{
public:
    boost::optional<Uri> parse(const std::string& rawUrl);

private:
    boost::optional<std::smatch> parseInternal(const std::string& rawUrl);

    Uri::Scheme getScheme(const std::string& scheme);
    Uri::Authority::HostType getHostType(const std::string& host);
    unsigned short getPortNumber(Uri::Scheme scheme, const std::string& port);

};
