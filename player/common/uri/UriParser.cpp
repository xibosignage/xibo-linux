#include "UriParser.hpp"
#include "UriParseError.hpp"

#include "logger/Logging.hpp"
#include <boost/format.hpp>

const std::regex UriRegex(R"(^([^:\/?#]+:\/\/)(?:(?:(.+(?::.+)?)@)?([^\/?#:]+)(?::(\d{1,5}))?)?(\/[^?#]*)?(\?[^#]*)?(#.*)?$)");
const std::regex IpRegex("(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])");

const std::map<std::string, Uri::Scheme> DefaultSchemes{{"https://", Uri::Scheme::HTTPS},
                                                         {"http://", Uri::Scheme::HTTP},
                                                         {"rtsp://", Uri::Scheme::RTSP},
                                                         {"file://", Uri::Scheme::FILE}};

const std::size_t UrlPartsCount = 5;

const std::size_t SchemePart = 1;
const std::size_t UserinfoPart = 2;
const std::size_t HostPart = 3;
const std::size_t PortPart = 4;
const std::size_t PathPart = 5;
const std::size_t QueryPart = 6;
const std::size_t FragmentPart = 7;

Uri UriParser::parse(const std::string& rawUri)
{
    auto urlParts = parseInternal(rawUri);

    auto scheme = getScheme(urlParts[SchemePart].str());
    auto userinfo = getUserInfo(urlParts[UserinfoPart].str());
    auto host = urlParts[HostPart].str();
    auto port = getPortNumber(urlParts[PortPart].str());
    auto path = getPath(urlParts[PathPart].str(), urlParts[QueryPart].str(), urlParts[FragmentPart].str());

    return Uri{scheme, Uri::Authority{userinfo, host, port}, path};
}

std::smatch UriParser::parseInternal(const std::string& rawUri)
{
    std::smatch urlMatch;
    auto valid = std::regex_match(rawUri, urlMatch, UriRegex) && urlMatch.size() > UrlPartsCount;
    if(valid)
    {
        return urlMatch;
    }

    throw UriParseError{"Uri is not valid: " + rawUri};
}

Uri::Scheme UriParser::getScheme(const std::string& scheme)
{
    try
    {
        return DefaultSchemes.at(scheme);
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

std::string UriParser::getPath(const std::string& target, const std::string& query, const std::string& fragment)
{
    std::string finalTarget = target.empty() ? "/" : target;

    return finalTarget + query + fragment;
}

Uri::Authority::HostType UriParser::getHostType(const std::string& host)
{
    if(host.empty()) return Uri::Authority::HostType::Invalid;

    std::smatch baseMatch;
    if(std::regex_match(host, baseMatch, IpRegex))
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
