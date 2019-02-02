#pragma once

#include <boost/optional/optional.hpp>

class Uri
{
public:    
    class Authority
    {
    public:
        enum class HostType
        {
            DNS,
            IP,
            Invalid
        };

        friend class Uri;

    private:
        Authority() = default;
        Authority(const std::string& host, boost::optional<unsigned short> port);
        Authority(const std::string& host);

        std::string m_host;
        HostType m_type = HostType::Invalid;
        boost::optional<unsigned short> m_port;
    };

    enum class Scheme
    {
        HTTP,
        HTTPS,
        File,
        Invalid
    };

    Uri(const std::string& rawUri);
    Uri(Scheme scheme, const std::string& host, boost::optional<unsigned short> port, const std::string& path);
    Uri(Scheme scheme, const std::string& host, const std::string& path);
    Uri(Scheme scheme, const std::string& path);

    Scheme scheme() const;
    const std::string& host() const;
    unsigned short port() const;
    Authority::HostType hostType() const;
    const std::string& path() const;

private:
    Scheme m_scheme = Scheme::Invalid;
    Authority m_authority;
    std::string m_path;
};

std::ostream& operator<<(std::ostream& out, const Uri& uri);
