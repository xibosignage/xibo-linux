#pragma once

#include <boost/optional/optional.hpp>

class FilePath;

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

        Authority(boost::optional<std::string> credentials,
                  const std::string& host,
                  boost::optional<unsigned short> optionalPort);
        Authority(const std::string& host);

        boost::optional<std::string> credentials() const;
        const std::string& host() const;
        HostType hostType() const;
        boost::optional<unsigned short> optionalPort() const;

    private:
        Authority() = default;

        boost::optional<std::string> userinfo_;
        std::string host_;
        HostType type_ = HostType::Invalid;
        boost::optional<unsigned short> port_;
    };

    enum class Scheme
    {
        HTTP,
        HTTPS,
        RTSP,
        File,
        Invalid
    };

    Uri() = default;  // TODO: remove default constructor

    static Uri fromString(const std::string& str);
    Uri(Scheme scheme, const Authority& authority, const std::string& path);  // TODO: strong type
    Uri(Scheme scheme, const std::string& host, const std::string& path);     // TODO: strong type
    Uri(const FilePath& path);

    bool isValid() const;
    std::string string() const;
    Scheme scheme() const;
    const std::string& host() const;
    unsigned short port() const;
    Authority::HostType hostType() const;
    const std::string& path() const;
    boost::optional<std::string> credentials() const;

private:
    std::string schemeToString(Scheme scheme) const;
    std::string portToString(boost::optional<unsigned short> port) const;
    static std::string removeEscapedSymbols(std::string url);

private:
    Scheme scheme_ = Scheme::Invalid;
    Authority authority_;
    std::string path_;
};

std::ostream& operator<<(std::ostream& out, const Uri& uri);
bool operator==(const Uri& first, const Uri& second);
