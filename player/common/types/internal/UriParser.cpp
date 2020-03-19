#include "UriParser.hpp"

#include <boost/format.hpp>

const std::regex UriRegex(
    R"(^([^:\/?#]+):\/\/(?:(?:(.+(?::.+)?)@)?([^\/?#:]+)(?::(\d{1,5}))?)?(\/[^?#]*)?(\?[^#]*)?(#.*)?$)");

const std::size_t UriPartsCount = 5;

const std::size_t SchemePart = 1;
const std::size_t UserinfoPart = 2;
const std::size_t HostPart = 3;
const std::size_t PortPart = 4;
const std::size_t TargetPart = 5;
const std::size_t QueryPart = 6;
const std::size_t FragmentPart = 7;

Uri UriParser::parse(const std::string& rawUri)
{
    auto urlParts = parseInternal(rawUri);

    auto scheme = schemeFrom(urlParts);
    auto authority = authorityFrom(urlParts);
    auto path = pathFrom(urlParts);

    if (authority)
    {
        return Uri{scheme, authority.value(), path};
    }

    return Uri{scheme, path};
}

std::smatch UriParser::parseInternal(const std::string& rawUri)
{
    std::smatch uriMatch;
    auto valid = std::regex_match(rawUri, uriMatch, UriRegex) && uriMatch.size() > UriPartsCount;
    if (valid)
    {
        return uriMatch;
    }

    throw UriParser::Error{"URI", "Parse error: " + rawUri};
}

Uri::Scheme UriParser::schemeFrom(const std::smatch& results)
{
    return Uri::Scheme{results[SchemePart].str()};
}

boost::optional<Uri::Authority> UriParser::authorityFrom(const std::smatch& results)
{
    auto host = hostFrom(results);
    if (host)
    {
        auto userinfo = userInfoFrom(results);
        auto port = portFrom(results);

        return Uri::Authority{userinfo, host.value(), port};
    }
    return {};
}

boost::optional<Uri::UserInfo> UriParser::userInfoFrom(const std::smatch& results)
{
    auto userinfo = results[UserinfoPart].str();
    if (userinfo.empty()) return {};

    return Uri::UserInfo::fromString(userinfo);
}

boost::optional<Uri::Host> UriParser::hostFrom(const std::smatch& results)
{
    auto host = results[HostPart].str();
    if (host.empty()) return {};

    return Uri::Host::fromString(host);
}

boost::optional<Uri::Port> UriParser::portFrom(const std::smatch& results)
{
    auto port = results[PortPart].str();
    auto scheme = Uri::Scheme{results[SchemePart].str()};
    if (!port.empty())
    {
        return Uri::Port{static_cast<unsigned short>(std::stoi(port))};
    }
    else if (DefaultPorts.count(scheme) > 0)
    {
        return Uri::Port::fromScheme(scheme);
    }
    return {};
}

std::string UriParser::pathFrom(const std::smatch& results)
{
    auto target = results[TargetPart].str();
    auto query = results[QueryPart].str();
    auto fragment = results[FragmentPart].str();
    std::string finalTarget = target.empty() ? "/" : target;

    return finalTarget + query + fragment;
}
