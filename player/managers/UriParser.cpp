#include "UriParser.hpp"

#include "utils/Logger.hpp"

const std::regex URL_REGEX(R"(([^:\/?#]+:\/\/)(?:((?:.+(?::.+)?@)?([^\/:]+))(?::(\d{1,5}))?)?(\/.*))");
const std::regex IP_REGEX("(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])");

const std::map<std::string, Uri::Scheme> DEFAULT_SCHEMES{{"https://", Uri::Scheme::HTTPS}, {"http://", Uri::Scheme::HTTP}, {"file://", Uri::Scheme::File}};
const std::map<Uri::Scheme, unsigned short> DEFAULT_PORTS{{Uri::Scheme::HTTPS, 443}, {Uri::Scheme::HTTP, 80}, {Uri::Scheme::File, 0}};

const std::size_t URL_PARTS_COUNT = 5;

const std::size_t SCHEME_PART = 1;
const std::size_t HOST_PART = 3;
const std::size_t PORT_PART = 4;
const std::size_t TARGET_PART = 5;

boost::optional<Uri> UriParser::parse(const std::string& rawUrl)
{
    auto urlPartsOpt = parseInternal(rawUrl);

    if(urlPartsOpt)
    {
        auto urlParts = *urlPartsOpt;
        Uri url;

        url.scheme = getScheme(urlParts[SCHEME_PART].str());
        url.authority.host = urlParts[HOST_PART].str();
        url.authority.hostType = getHostType(url.authority.host);
        url.authority.port = getPortNumber(url.scheme, urlParts[PORT_PART].str());
        url.path = urlParts[TARGET_PART].str();

        return std::move(url);
    }

    return {};
}

boost::optional<std::smatch> UriParser::parseInternal(const std::string& rawUrl)
{
    std::smatch urlMatch;

    auto valid = std::regex_match(rawUrl, urlMatch, URL_REGEX) && urlMatch.size() > URL_PARTS_COUNT;
    if(valid)
    {
        return std::move(urlMatch);
    }
    else
    {
        return {};
    }
}

Uri::Scheme UriParser::getScheme(const std::string& scheme)
{
    return DEFAULT_SCHEMES.at(scheme);
}

unsigned short UriParser::getPortNumber(Uri::Scheme scheme, const std::string& port)
{
    if(!port.empty())
    {
        return static_cast<unsigned short>(std::stoi(port));
    }
    else
    {
        return DEFAULT_PORTS.at(scheme);
    }
}

Uri::Authority::HostType UriParser::getHostType(const std::string& host)
{
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
