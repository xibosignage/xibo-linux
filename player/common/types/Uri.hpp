#pragma once

#include "common/PlayerRuntimeError.hpp"
#include "common/types/internal/StrongType.hpp"

#include <boost/optional/optional.hpp>
#include <map>

class FilePath;

class Uri
{
public:
    class Host : public StrongType<std::string>
    {
    public:
        enum class Type
        {
            DNS,
            IP
        };

        struct Error : PlayerRuntimeError
        {
            using PlayerRuntimeError::PlayerRuntimeError;
        };

        static Host fromString(const std::string& optionalHost);
        Type type() const;

    private:
        Host(const std::string& optionalHost, Type type);

    private:
        Type type_;
    };

    struct Scheme : StrongType<std::string>
    {
        explicit Scheme(const std::string& scheme);
    };

    static inline const Scheme FileScheme{"file"};
    static inline const Scheme HttpScheme{"http"};
    static inline const Scheme HttpsScheme{"https"};

    class Port : public StrongType<unsigned short>
    {
    public:
        struct Error : PlayerRuntimeError
        {
            using PlayerRuntimeError::PlayerRuntimeError;
        };

        explicit Port(unsigned short value);
        static Port fromScheme(const Uri::Scheme& scheme);

        std::string string() const;
    };

    class UserInfo : public StrongType<std::string>
    {
    public:
        struct Error : PlayerRuntimeError
        {
            using PlayerRuntimeError::PlayerRuntimeError;
        };

        UserInfo(const std::string& username, const std::string& password);
        static UserInfo fromString(const std::string& userInfo);

        const std::string& username() const;
        const std::string& password() const;

    private:
        std::string username_;
        std::string password_;
    };

    class Authority
    {
    public:
        Authority(const boost::optional<UserInfo>& info, const Host& host, const boost::optional<Port>& optionalPort);

        UserInfo userInfo() const;
        boost::optional<UserInfo> optionalUserInfo() const;
        const Host& host() const;
        boost::optional<Port> optionalPort() const;
        Port port() const;

    private:
        boost::optional<UserInfo> userInfo_;
        Host host_;
        boost::optional<Port> port_;
    };

    static Uri fromString(const std::string& str);
    static Uri fromFile(const FilePath& path);

    Uri(const Scheme& scheme, const Authority& authority, const std::string& path);
    Uri(const Scheme& scheme, const Host& optionalHost, const std::string& path);
    Uri(const Scheme& scheme, const std::string& path);

    std::string string() const;
    boost::optional<Authority> optionalAuthority() const;
    Authority authority() const;
    const Scheme& scheme() const;
    // TODO: StrongType
    const std::string& path() const;

private:
    static std::string unescape(const std::string &input);

private:
    Scheme scheme_;
    boost::optional<Authority> authority_;
    std::string path_;
};

const std::map<Uri::Scheme, Uri::Port> DefaultPorts{{Uri::Scheme{"http"}, Uri::Port{80}},
                                                    {Uri::Scheme{"https"}, Uri::Port{443}}};

std::ostream& operator<<(std::ostream& out, const Uri& uri);
bool operator==(const Uri& first, const Uri& second);
bool operator!=(const Uri& first, const Uri& second);

bool operator==(const Uri::Host& first, const Uri::Host& second);
bool operator!=(const Uri::Host& first, const Uri::Host& second);

bool operator==(const Uri::Scheme& first, const Uri::Scheme& second);
bool operator!=(const Uri::Scheme& first, const Uri::Scheme& second);
bool operator<(const Uri::Scheme& first, const Uri::Scheme& second);

bool operator==(const Uri::Port& first, const Uri::Port& second);
bool operator!=(const Uri::Port& first, const Uri::Port& second);

bool operator==(const Uri::UserInfo& first, const Uri::UserInfo& second);
bool operator!=(const Uri::UserInfo& first, const Uri::UserInfo& second);
