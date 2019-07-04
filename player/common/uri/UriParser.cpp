#include "UriParser.hpp"
#include "UriParseError.hpp"

#include "logger/Logging.hpp"
#include <boost/format.hpp>

const std::regex URI_REGEX(R"(([^:\/?#]+:\/\/)(?:(?:(.+(?::.+)?)@)?([^\/:]+)(?::(\d{1,5}))?)?([\/?].*))");
const std::regex IP_REGEX("(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])");

const std::map<std::string, Uri::Scheme> DEFAULT_SCHEMES{{"https://", Uri::Scheme::HTTPS},
                                                         {"http://", Uri::Scheme::HTTP},
                                                         {"rtsp://", Uri::Scheme::RTSP},
                                                         {"file://", Uri::Scheme::File}};

const std::size_t URL_PARTS_COUNT = 5;

const std::size_t SCHEME_PART = 1;
const std::size_t USERINFO_PART = 2;
const std::size_t HOST_PART = 3;
const std::size_t PORT_PART = 4;
const std::size_t PATH_PART = 5;

Uri UriParser::parse(const std::string& rawUri)
{
    auto urlParts = parseInternal(rawUri);

    auto scheme = getScheme(urlParts[SCHEME_PART].str());
    auto userinfo = getUserInfo(urlParts[USERINFO_PART].str());
    auto host = urlParts[HOST_PART].str();
    auto port = getPortNumber(urlParts[PORT_PART].str());
    auto path = urlParts[PATH_PART].str();

    return Uri{scheme, Uri::Authority{userinfo, host, port}, path};
}

std::smatch UriParser::parseInternal(const std::string& rawUri)
{
    std::smatch urlMatch;
    auto valid = std::regex_match(rawUri, urlMatch, URI_REGEX) && urlMatch.size() > URL_PARTS_COUNT;
    if(valid)
    {
        return urlMatch;
    }

    throw UriParseError{"Uri is not valid"};
}

Uri::Scheme UriParser::getScheme(const std::string& scheme)
{
    try
    {
        return DEFAULT_SCHEMES.at(scheme);
    }
    catch(std::out_of_range&)
    {
        auto formatted = boost::format("%1% scheme is not supported") % scheme;
        throw UriParseError{formatted.str()};
    }
}

boost::optional<unsigned short> UriParser::getPortNumber(const std::string& port)
{
    if(!port.empty())
    {
        return static_cast<unsigned short>(std::stoi(port));
    }
    return {};
}

Uri::Authority::HostType UriParser::getHostType(const std::string& host)
{
    if(host.empty()) return Uri::Authority::HostType::Invalid;

    std::smatch baseMatch;
    if(std::regex_match(host, baseMatch, IP_REGEX))
    {
        return Uri::Authority::HostType::IP;
    }
    else
    {
        return Uri::Authority::HostType::DNS;
    }
}

boost::optional<std::string> UriParser::getUserInfo(const std::string& userinfo)
{
    if(userinfo.empty()) return {};

    return userinfo;
}
