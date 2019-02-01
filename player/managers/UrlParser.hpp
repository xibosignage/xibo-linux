#pragma once

#include <string>
#include <regex>
#include <boost/optional/optional.hpp>

#include "Url.hpp"

class UrlParser
{
public:
    boost::optional<Url> parse(const std::string& rawUrl);

private:
    struct SplittedUrl
    {
        std::string scheme;
        std::string host;
        std::string port;
        std::string target;
    };

    boost::optional<std::smatch> parseInternal(const std::string& rawUrl);

    Url::Scheme getScheme(const std::string& scheme);
    unsigned short getPortNumber(Url::Scheme scheme, const std::string& port);
    Url::Host getHost(const std::string& host);
    Url::Host::Type getHostType(const std::string& host);

};
