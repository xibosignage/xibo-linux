#include "UrlParser.hpp"

#include "utils/Logger.hpp"

const std::regex URL_REGEX(R"((https?:\/\/)([^:]+)(?::(\d{1,5}))?(\/.*))");
const std::regex IP_REGEX("(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])");

const std::map<std::string, Url::Scheme> DEFAULT_SCHEMES{{"https://", Url::Scheme::HTTPS}, {"http://", Url::Scheme::HTTP}};
const std::map<Url::Scheme, unsigned short> DEFAULT_PORTS{{Url::Scheme::HTTPS, 443}, {Url::Scheme::HTTP, 80}};

const std::size_t URL_PARTS_COUNT = 4;

const std::size_t SCHEME_PART = 1;
const std::size_t HOST_PART = 2;
const std::size_t PORT_PART = 3;
const std::size_t TARGET_PART = 4;

boost::optional<Url> UrlParser::parse(const std::string& rawUrl)
{
    auto urlPartsOpt = parseInternal(rawUrl);

    if(urlPartsOpt)
    {
        auto urlParts = *urlPartsOpt;
        Url url;

        url.scheme = getScheme(urlParts[SCHEME_PART].str());
        url.host = getHost(urlParts[HOST_PART].str());
        url.port = getPortNumber(url.scheme, urlParts[PORT_PART].str());
        url.target = urlParts[TARGET_PART].str();

        return std::move(url);
    }

    return {};
}

boost::optional<std::smatch> UrlParser::parseInternal(const std::string& rawUrl)
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

Url::Scheme UrlParser::getScheme(const std::string& scheme)
{
    return DEFAULT_SCHEMES.at(scheme);
}

unsigned short UrlParser::getPortNumber(Url::Scheme scheme, const std::string& port)
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

Url::Host UrlParser::getHost(const std::string& host)
{
    return Url::Host{host, getHostType(host)};
}

Url::Host::Type UrlParser::getHostType(const std::string& host)
{
    std::smatch baseMatch;
    if(std::regex_match(host, baseMatch, IP_REGEX))
    {
        return Url::Host::Type::IP;
    }
    else
    {
        return Url::Host::Type::DNS;
    }
}
