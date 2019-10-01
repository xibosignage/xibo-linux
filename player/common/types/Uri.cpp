#include "Uri.hpp"
#include "UriParser.hpp"
#include "Utils.hpp"

#include "common/fs/FilePath.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <iostream>

const std::map<Uri::Scheme, unsigned short> DefaultPorts{{Uri::Scheme::HTTPS, 443}, {Uri::Scheme::HTTP, 80}};

template <>
std::string Utils::toString(Uri::Scheme val)
{
    switch (val)
    {
        case Uri::Scheme::HTTP: return "HTTP";
        case Uri::Scheme::HTTPS: return "HTTPS";
        case Uri::Scheme::RTSP: return "RTSP";
        case Uri::Scheme::File: return "FILE";
    }

    return "unknown";
}

template <>
std::string Utils::toString(Uri::Authority::HostType val)
{
    switch (val)
    {
        case Uri::Authority::HostType::DNS: return "DNS";
        case Uri::Authority::HostType::IP: return "IP";
        case Uri::Authority::HostType::Invalid: return "Invalid";
    }

    return "unknown";
}

Uri::Authority::Authority(boost::optional<std::string> userinfo,
                          const std::string& host,
                          boost::optional<unsigned short> port) :
    userinfo_(userinfo),
    host_{host},
    port_{port}
{
    type_ = UriParser::getHostType(host);
}

Uri::Authority::Authority(const std::string& host) : Authority{{}, host, {}} {}

boost::optional<std::string> Uri::Authority::credentials() const
{
    return userinfo_;
}

const std::string& Uri::Authority::host() const
{
    return host_;
}

Uri::Authority::HostType Uri::Authority::hostType() const
{
    return type_;
}

boost::optional<unsigned short> Uri::Authority::optionalPort() const
{
    return port_;
}

std::string Uri::removeEscapedSymbols(std::string url)
{
    //    boost::replace_all(url, "%21", "!");
    //    boost::replace_all(url, "%23", "#");
    //    boost::replace_all(url, "%24", "$");
    //    boost::replace_all(url, "%25", "%");
    //    boost::replace_all(url, "%26", "&");
    //    boost::replace_all(url, "%27", "'");
    //    boost::replace_all(url, "%28", "(");
    //    boost::replace_all(url, "%29", ")");
    //    boost::replace_all(url, "%2A", "*");
    //    boost::replace_all(url, "%2B", "+");
    //    boost::replace_all(url, "%2C", ",");
    boost::replace_all(url, "%2F", "/");
    boost::replace_all(url, "%3A", ":");
    //    boost::replace_all(url, "%3B", ";");
    //    boost::replace_all(url, "%3D", "=");
    //    boost::replace_all(url, "%3F", "?");
    //    boost::replace_all(url, "%40", "@");
    //    boost::replace_all(url, "%5B", "]");
    //    boost::replace_all(url, "%5D", "[");

    return url;
}

Uri Uri::fromString(const std::string& str)
{
    return UriParser::parse(removeEscapedSymbols(str));
}

Uri::Uri(Uri::Scheme scheme, const Authority& authority, const std::string& path) :
    scheme_{scheme},
    authority_{authority},
    path_{path}
{
}

Uri::Uri(Uri::Scheme scheme, const std::string& host, const std::string& path) : Uri{scheme, Authority{host}, path} {}

Uri::Uri(const FilePath& path) : Uri{Uri::Scheme::File, Authority{}, path.string()} {}

bool Uri::isValid() const
{
    return scheme_ != Scheme::Invalid;
}

std::string Uri::string() const
{
    std::string uri;

    uri += schemeToString(scheme_);
    if (authority_.credentials())
    {
        uri += authority_.credentials().value() + "@";
    }
    uri += authority_.host();
    if (authority_.optionalPort())
    {
        uri += ":" + portToString(authority_.optionalPort());
    }
    uri += path_;

    return uri;
}

Uri::Scheme Uri::scheme() const
{
    return scheme_;
}

const std::string& Uri::host() const
{
    return authority_.host();
}

unsigned short Uri::port() const
{
    assert(scheme_ == Scheme::HTTP || scheme_ == Scheme::HTTPS);

    auto optPort = authority_.optionalPort();

    return optPort ? optPort.value() : DefaultPorts.at(scheme_);
}

Uri::Authority::HostType Uri::hostType() const
{
    return authority_.hostType();
}

const std::string& Uri::path() const
{
    return path_;
}

boost::optional<std::string> Uri::credentials() const
{
    return authority_.credentials();
}

std::string Uri::schemeToString(Uri::Scheme scheme) const
{
    switch (scheme)
    {
        case Uri::Scheme::File: return "file://";
        case Uri::Scheme::HTTP: return "http://";
        case Uri::Scheme::HTTPS: return "https://";
        case Uri::Scheme::RTSP: return "rtsp://";
        default: return {};
    }
}

std::string Uri::portToString(boost::optional<unsigned short> port) const
{
    assert(port.has_value());

    return std::to_string(port.value());
}

std::ostream& operator<<(std::ostream& out, const Uri& uri)
{
    return out << "Scheme: " << Utils::toString(uri.scheme()) << " HostType: " << Utils::toString(uri.hostType())
               << " Host: " << uri.host() << " Port: " << uri.port() << " Target: " << uri.path();
}

bool operator==(const Uri& first, const Uri& second)
{
    return first.string() == second.string();
}
