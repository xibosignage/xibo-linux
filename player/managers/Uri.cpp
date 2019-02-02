#include "Uri.hpp"
#include "UriParser.hpp"

#include <iostream>

Uri::Authority::Authority(const std::string& host, boost::optional<unsigned short> port) :
    m_host{host}, m_port{port}
{
    m_type = UriParser{}.getHostType(host);
}

Uri::Authority::Authority(const std::string& host) :
    Authority{host, {}}
{

}


Uri::Uri(const std::string& rawUri) : Uri(UriParser().parse(rawUri))
{
}

Uri::Uri(Uri::Scheme scheme, const std::string& host, boost::optional<unsigned short> port, const std::string& path) :
    m_scheme{scheme}, m_authority{Authority{host, port}}, m_path{path}
{

}

Uri::Uri(Uri::Scheme scheme, const std::string& host, const std::string& path) :
    Uri{scheme, host, {}, path}
{

}

Uri::Uri(Uri::Scheme scheme, const std::string& path) :
    Uri{scheme, {}, {}, path}
{

}

Uri::Scheme Uri::scheme() const
{
    return m_scheme;
}

const std::string& Uri::host() const
{
    return m_authority.m_host;
}

unsigned short Uri::port() const
{
    if(m_scheme != Scheme::HTTP && m_scheme != Scheme::HTTPS)
        throw std::logic_error("port is not support on this scheme");

    return m_authority.m_port.value();
}

Uri::Authority::HostType Uri::hostType() const
{
    return m_authority.m_type;
}

const std::string& Uri::path() const
{
    return m_path;
}

std::ostream& operator<<(std::ostream& out, const Uri& uri)
{
    return out << "Scheme: " <<  static_cast<int>(uri.scheme())
               << " HostType: " <<  static_cast<int>(uri.hostType())
               << " Host: " << uri.host()
               << " Port: " << uri.port()
               << " Target: " << uri.path();
}
