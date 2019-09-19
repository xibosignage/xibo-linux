#include "Uri.hpp"
#include "UriParser.hpp"
#include "Utils.hpp"

#include <iostream>
#include <boost/algorithm/string/replace.hpp>

const std::map<Uri::Scheme, unsigned short> DefaultPorts{{Uri::Scheme::HTTPS, 443}, {Uri::Scheme::HTTP, 80}};

template<>
std::string Utils::toString(Uri::Scheme val)
{
    switch (val)
    {
        case Uri::Scheme::HTTP: return "HTTP";
        case Uri::Scheme::HTTPS: return "HTTPS";
        case Uri::Scheme::RTSP: return "RTSP";
        case Uri::Scheme::FILE: return "FILE";
        case Uri::Scheme::Invalid: return "Invalid";
    }

    return "unknown";
}

template<>
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

Uri::Authority::Authority(boost::optional<std::string> userinfo, const std::string& host, boost::optional<unsigned short> port) :
    m_userinfo(userinfo), m_host{host}, m_port{port}
{
    m_type = UriParser::getHostType(host);
}

Uri::Authority::Authority(const std::string& host) :
    Authority{{}, host, {}}
{
}

boost::optional<std::string> Uri::Authority::credentials() const
{
    return m_userinfo;
}

const std::string& Uri::Authority::host() const
{
    return m_host;
}

Uri::Authority::HostType Uri::Authority::hostType() const
{
    return m_type;
}

boost::optional<unsigned short> Uri::Authority::optionalPort() const
{
    return m_port;
}

Uri::Uri(const std::string& rawUri) : Uri(UriParser::parse(removeEscapedSymbols(rawUri)))
{
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

Uri::Uri(Uri::Scheme scheme, const Authority& authority, const std::string& path) :
    m_scheme{scheme}, m_authority{authority}, m_path{path}
{
}

Uri::Uri(Uri::Scheme scheme, const std::string& host, const std::string& path) :
    Uri{scheme, Authority{host}, path}
{
}

Uri::Uri(Uri::Scheme scheme, const std::string& path) :
    Uri{scheme, Authority{}, path}
{
}

bool Uri::isValid() const
{
    return m_scheme != Scheme::Invalid;
}

std::string Uri::string() const
{
    std::string uri;

    uri += schemeToString(m_scheme);
    if(m_authority.credentials())
    {
        uri += m_authority.credentials().value() + "@";
    }
    uri += m_authority.host();
    if(m_authority.optionalPort())
    {
        uri += ":" + portToString(m_authority.optionalPort());
    }
    uri += m_path;

    return uri;
}

Uri::Scheme Uri::scheme() const
{
    return m_scheme;
}

const std::string& Uri::host() const
{
    return m_authority.host();
}

unsigned short Uri::port() const
{
    assert(m_scheme == Scheme::HTTP || m_scheme == Scheme::HTTPS);

    auto optPort = m_authority.optionalPort();

    return optPort ? optPort.value() : DefaultPorts.at(m_scheme);
}

Uri::Authority::HostType Uri::hostType() const
{
    return m_authority.hostType();
}

const std::string& Uri::path() const
{
    return m_path;
}

boost::optional<std::string> Uri::credentials() const
{
    return m_authority.credentials();
}

std::string Uri::schemeToString(Uri::Scheme scheme) const
{
    switch(scheme)
    {
        case Uri::Scheme::FILE:
            return "file://";
        case Uri::Scheme::HTTP:
            return "http://";
        case Uri::Scheme::HTTPS:
            return "https://";
        case Uri::Scheme::RTSP:
            return "rtsp://";
        default:
            return {};
    }
}

std::string Uri::portToString(boost::optional<unsigned short> port) const
{
    assert(port.has_value());

    return std::to_string(port.value());
}

std::ostream& operator <<(std::ostream& out, const Uri& uri)
{
    return out << "Scheme: " << Utils::toString(uri.scheme())
               << " HostType: " << Utils::toString(uri.hostType())
               << " Host: " << uri.host()
               << " Port: " << uri.port()
               << " Target: " << uri.path();
}

bool operator ==(const Uri& first, const Uri& second)
{
    return first.string() == second.string();
}
